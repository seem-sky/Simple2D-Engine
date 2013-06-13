#include "MapIO.h"
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>

using namespace MAP;
using namespace XML_IO;

/*#####
# MapReader
#####*/
bool MapReader::checkoutChildren(const QDomNode &parentNode)
{
    bool tileResult = false, objectResult = false;
    _loadTiles(parentNode, tileResult);
    _loadObjects(parentNode, objectResult);
    if (tileResult && objectResult)
        return true;
    return false;
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
    for (uint32 j = 0; j < layerChildren.length() && j < m_pMap->getLayerSize(layer); ++j)
    {
        QDomNode layerNode = layerChildren.at(j);
        QDomNodeList rows = layerNode.childNodes();
        // iterate through rows
        for (uint32 i = 0; i < rows.length() && i < m_pMap->getSize().y; ++i)
        {
            if (rows.at(i).isNull())
                continue;
            QDomNamedNodeMap attributes = rows.at(i).attributes();
            if (rows.at(i).hasAttributes())
                _parseTileString(Point<uint32>(i, j), rows.at(i).attributes().item(0).nodeValue(), layer);
        }
    }
}

void MapReader::_parseTileString(const Point<uint32> &pos, QString &tileString, Layer layer)
{
    for (uint32 x = 0; x < m_pMap->getSize().x && !tileString.isEmpty(); ++x)
    {
        bool tileStored = false;
        while (!tileStored && !tileString.isEmpty())
        {
            // if first sign is a number
            if (tileString.at(0) <= 57 && tileString.at(0) >= 48)
            {
                MapTile tileObject;
                uint32 uiColonIndex = tileString.indexOf(','), uiDblPointIndex = tileString.indexOf(':');
                tileObject.m_uiTileID = tileString.left(uiColonIndex < uiDblPointIndex ? uiColonIndex : uiDblPointIndex).toUInt();
                if (uiDblPointIndex < uiColonIndex)
                {
                    tileString.remove(0, uiDblPointIndex != -1 ? uiDblPointIndex+1 : uiDblPointIndex);
                    uiColonIndex = tileString.indexOf(',');
                    tileObject.m_uiAutoTileSetID = tileString.left(uiColonIndex).toUInt();
                }
                tileString.remove(0, uiColonIndex != -1 ? uiColonIndex+1 : uiColonIndex);
                m_pMap->setMapTile(Point3D<uint32>(x, pos.x, pos.y), tileObject, layer);
                tileStored = true;
            }
            // if first sign is a not a number, delete it
            else
                tileString.remove(0, 1);
        }
    }
}

void MapReader::_loadObjects(const QDomNode &parentNode, bool &result)
{
    result = false;
    if (!m_pMap)
        return;

    m_pMap->m_Objects.clear();
    QDomNode node = getSingleNode(parentNode, "ObjectData");
    if (node.isNull())
        return;

    // get objects
    QDomNodeList objects = node.childNodes();
    for (uint32 j = 0; j < objects.length(); j++)
    {
        QDomNode currentObject = objects.at(j);
        if (currentObject.isNull())
            continue;
        MapObjectPtr newObject(new MapObject());
        QDomNamedNodeMap objectAttributes = currentObject.attributes();
        // parse attributes
        for (uint32 k = 0; k < objectAttributes.length(); k++)
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
                newObject->m_Position.x = attribute.nodeValue().toUInt();
            else if (attribute.nodeName() == "YPos")
                newObject->m_Position.y = attribute.nodeValue().toUInt();
            else if (attribute.nodeName() == "Direction")
                newObject->m_Direction = static_cast<MAP::MapDirection>(attribute.nodeValue().toUInt());
            else if (attribute.nodeName() == "Layer")
                newObject->m_Layer = static_cast<MAP::MapObjectLayer>(attribute.nodeValue().toUInt());
        }
        m_pMap->addMapObject(newObject);
    }
    result = true;
}

/*#####
# MapWriter
#####*/
bool MapWriter::_writeChildren(QXmlStreamWriter &writer)
{
    _storeTiles(writer);
    _storeObjects(writer);
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
                MapTile tile = m_pMap->getMapTile(Point3D<uint32>(x, y, layer), curLayer);
                line.append(QString::number(tile.m_uiTileID) + ':' + QString::number(tile.m_uiAutoTileSetID) + ',');
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