#include "MapIO.h"
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <QtCore/QTime>

using namespace MAP;
using namespace XML_IO;

const QString TILE_AUTOTILE_DELIMITER = ":";
const QString TILE_OBJECT_DELIMITER = ",";

/*#####
# MapReader
#####*/
bool MapReader::checkoutChildren(const QDomNode &parentNode)
{
    BASIC_LOG("Map load start: ID:" + QString::number(m_pMap->getID()) + " file name:" + m_pMap->getFileName() + ".");
    QTime t;
    t.start();
    bool tileResult = false, objectResult = false;
    _loadTiles(parentNode, tileResult);
    _loadObjects(parentNode, objectResult);
    if (tileResult && objectResult)
        BASIC_LOG("Map load succeeded after " + QString::number(t.elapsed()) + "msec.");
    else
        ERROR_LOG("Map load failed.");
    return tileResult && objectResult;
}

void MapReader::_loadTiles(const QDomNode &parentNode, bool &result)
{
    result = false;
    if (!m_pMap)
        return;

    // resize map data
    m_pMap->_clearTiles();
    m_pMap->_resizeMap(m_pMap->getSize(), m_pMap->getLayerSize(LAYER_FOREGROUND), m_pMap->getLayerSize(LAYER_BACKGROUND));
    m_pMap->m_DataLoaded = true;

    // load background
    QDomNode node = getSingleNode(parentNode, "LayerBackground");
    if (!node.isNull())
        _loadTiles(node, LAYER_BACKGROUND);
    // load foreground
    node = getSingleNode(parentNode, "LayerForeground");
    if (!node.isNull())
        _loadTiles(node, LAYER_FOREGROUND);
    result = true;
}

void MapReader::_loadTiles(const QDomNode &parentNode, Layer layer)
{
    QDomNodeList layerChildren = parentNode.childNodes();
    for (uint32 j = 0; j < static_cast<uint32>(layerChildren.length()) && j < m_pMap->getLayerSize(layer); ++j)
    {
        QDomNode layerNode = layerChildren.at(j);
        QDomNodeList rows = layerNode.childNodes();
        // iterate through rows
        for (uint32 i = 0; i < static_cast<uint32>(rows.length()) && i < m_pMap->getSize().y; ++i)
        {
            if (rows.at(i).isNull())
                continue;
            QDomNamedNodeMap attributes = rows.at(i).attributes();
            if (rows.at(i).hasAttributes())
                _parseTileString(UInt32Point(i, j), rows.at(i).attributes().item(0).nodeValue(), layer);
        }
    }
}

void MapReader::_parseTileString(const UInt32Point &pos, const QString &tileString, Layer layer)
{
    QStringList tileObjectStrings = tileString.split(TILE_OBJECT_DELIMITER);
    QStringList::const_iterator itr = tileObjectStrings.begin();
    for (uint32 x = 0; x < m_pMap->getSize().x && itr != tileObjectStrings.end(); ++x, ++itr)
    {
        int delimiterIndex = itr->indexOf(TILE_AUTOTILE_DELIMITER);
        MapTile tileObject(itr->left(delimiterIndex).toUInt(), itr->right(itr->length() -1 -delimiterIndex).toUInt());
        m_pMap->setMapTile(UInt32Point3D(x, pos.x, pos.y), tileObject, layer);
    }
}

void MapReader::_loadObjects(const QDomNode &parentNode, bool &result)
{
    result = false;
    if (!m_pMap)
        return;

    m_pMap->m_Objects.clear();
    result = true;
    QDomNode node = getSingleNode(parentNode, "ObjectData");
    if (node.isNull())
        return;

    // get objects
    QDomNodeList objects = node.childNodes();
    for (int32 j = 0; j < objects.length(); j++)
    {
        QDomNode currentObject = objects.at(j);
        if (currentObject.isNull())
            continue;
        MapObjectPtr newObject(new MapObject());
        QDomNamedNodeMap objectAttributes = currentObject.attributes();
        // parse attributes
        for (int32 k = 0; k < objectAttributes.length(); k++)
        {
            QDomNode attribute = objectAttributes.item(k);
            if (attribute.isNull())
                continue;
            if (attribute.nodeName() == "ID")
                newObject->m_ObjectID = attribute.nodeValue().toUInt();
            else if (attribute.nodeName() == "GUID")
                newObject->m_GUID = attribute.nodeValue().toUInt();
            else if (attribute.nodeName() == "Type")
                newObject->m_Type = static_cast<DATABASE::ObjectType>(attribute.nodeValue().toUInt());
            else if (attribute.nodeName() == "XPos")
                newObject->m_Position.x = attribute.nodeValue().toInt();
            else if (attribute.nodeName() == "YPos")
                newObject->m_Position.y = attribute.nodeValue().toInt();
            else if (attribute.nodeName() == "Direction")
                newObject->m_Direction = static_cast<MAP::MapDirection>(attribute.nodeValue().toUInt());
            else if (attribute.nodeName() == "Layer")
                newObject->m_Layer = static_cast<MAP::MapObjectLayer>(attribute.nodeValue().toUInt());
        }
        m_pMap->addMapObject(newObject);
    }
    BASIC_LOG("Added " + QString::number(m_pMap->getMapObjectCount()) + " objects to map.");
}

/*#####
# MapWriter
#####*/
bool MapWriter::_writeChildren(QXmlStreamWriter &writer)
{
    BASIC_LOG("Map write start: ID:" + QString::number(m_pMap->getID()) + " file name:" + m_pMap->getFileName() + ".");
    QTime t;
    t.start();
    _storeTiles(writer);
    _storeObjects(writer);
    BASIC_LOG("Map write succeeded after " + QString::number(t.elapsed()) + "msec.");
    return true;
}

void MapWriter::_storeTiles(QXmlStreamWriter &writer)
{
    writer.writeStartElement("LayerBackground");
    _storeTiles(writer, LAYER_BACKGROUND);
    writer.writeEndElement();
    writer.writeStartElement("LayerForeground");
    _storeTiles(writer, LAYER_FOREGROUND);
    writer.writeEndElement();
}

void MapWriter::_storeTiles(QXmlStreamWriter &writer, Layer curLayer)
{
    for (uint32 layer = 0; layer < m_pMap->getLayerSize(curLayer); ++layer)
    {
        writer.writeStartElement("Layer");
        for (uint32 y = 0; y < m_pMap->getSize().y; ++y)
        {
            writer.writeEmptyElement("MapTiles");
            QString line;
            for (uint32 x = 0; x < m_pMap->getSize().x; ++x)
            {
                MapTile tile = m_pMap->getMapTile(UInt32Point3D(x, y, layer), curLayer);
                line.append(QString::number(tile.m_uiTileID) + TILE_AUTOTILE_DELIMITER + QString::number(tile.m_uiAutoTileSetID) + TILE_OBJECT_DELIMITER);
            }
            writer.writeAttribute("Tiles", line);
        }
        writer.writeEndElement();
    }
}

void MapWriter::_storeObjects(QXmlStreamWriter &writer)
{
    writer.writeStartElement("ObjectData");
    for (uint32 i = 1; i <= m_pMap->getMapObjectCount(); ++i)
    {
        ConstMapObjectPtr obj;
        if (m_pMap->getMapObject(i, obj))
            _getXMLDataFromObject(obj, writer);
    }
    writer.writeEndElement();
}

void MapWriter::_getXMLDataFromObject(ConstMapObjectPtr obj, QXmlStreamWriter &writer)
{
    if (!obj)
        return;
    writer.writeEmptyElement("Object");
    writer.writeAttribute("ID", QString::number(obj->m_ObjectID));
    writer.writeAttribute("Type", QString::number(obj->m_Type));
    writer.writeAttribute("GUID", QString::number(obj->m_GUID));
    writer.writeAttribute("XPos", QString::number(obj->m_Position.x));
    writer.writeAttribute("YPos", QString::number(obj->m_Position.y));
    writer.writeAttribute("Direction", QString::number(obj->m_Direction));
    writer.writeAttribute("Layer", QString::number(obj->m_Layer));
}