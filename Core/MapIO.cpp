#include "MapIO.h"
#include <atlbase.h>
#include <msxml.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>


#import <msxml6.dll>

using namespace MAP;
using namespace XML_IO;

/*#####
# MapReader
#####*/
//void MapReader::loadMapThreaded(const QString &sFileName, bool &result)
//{
//    result = false;
//    if (STATE_NONE != m_State)
//        return;
//
//    m_State = STATE_IN_PROGRESS;
//    bool tileResult = false, objectResult = false;
//    boost::thread loadTiles(boost::bind(&MapReader::_loadTiles, this, boost::ref(tileResult), boost::cref(sFileName)));
//    boost::thread loadObjects(boost::bind(&MapReader::_loadObjects, this, boost::ref(objectResult), boost::cref(sFileName)));
//    loadTiles.join();
//    loadObjects.join();
//    m_State = STATE_DONE;
//    if (tileResult && objectResult)
//        result = true;
//}

bool MapReader::checkoutChildren(const QDomNode &parentNode)
{
    bool tileResult = false, objectResult = false;
    _loadTiles(parentNode, tileResult);
    _loadObjects(parentNode, objectResult);
    m_State = STATE_DONE;
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
    m_pMap->_clear();
    m_pMap->_resizeMap(m_pMap->getSize());

    StdString2DVector TileByRowFromFileList;
    QDomNode node = getSingleNode(parentNode, "MapData");
    if (node.isNull())
        return;
    QDomNodeList layerChildren = node.childNodes();
    for (uint32 j = 0; j < layerChildren.length() && j < m_pMap->getSize().z; ++j)
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
                _parseTileString(Point<uint32>(i, j), rows.at(i).attributes().item(0).nodeValue());
        }
    }
    m_pMap->m_DataLoaded = true;
    result = true;
}

void MapReader::_parseTileString(const Point<uint32> &pos, QString &tileString)
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
                m_pMap->m_MapData[x][pos.x][pos.y] = tileObject;
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

    // read out layers
    QDomNodeList layers = node.childNodes();
    for (uint32 i = 0; i < layers.length(); i++)
    {
        QDomNode currentLayer = layers.at(i);
        if (currentLayer.isNull())
            continue;
        MapObjectPtrVector vObjects;
        // get objects
        QDomNodeList objects = currentLayer.childNodes();
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
                else if (attribute.nodeName() == "XPos")
                    newObject->m_Position.x = attribute.nodeValue().toUInt();
                else if (attribute.nodeName() == "YPos")
                    newObject->m_Position.y = attribute.nodeValue().toUInt();
                else if (attribute.nodeName() == "Direction")
                    newObject->m_Direction = static_cast<MAP::MAP_DIRECTION>(attribute.nodeValue().toUInt());
            }
            vObjects.push_back(newObject);
        }
        //m_pMap->m_Objects.push_back(vObjects);
    }
    result = true;
}

/*#####
# MapWriter
#####*/
//void MapWriter::_storeTiles(XML::XML_WriteData &xmlResult)
//{
//    for (uint32 layer = 0; layer < m_pMap->getSize().z; ++layer)
//    {
//        XML::XML_WriteData xml_Layer(XML::XML_WRITE_APPEND);
//        for (uint32 y = 0; y < m_pMap->getSize().y; ++y)
//        {
//            XML::XML_WriteData xml_Line(XML::XML_WRITE_APPEND);
//            QString sLine;
//            for (uint32 x = 0; x < m_pMap->getSize().x; ++x)
//                sLine.append(ToString(m_pMap->m_MapData[x][y][layer].m_uiTileID) + ':' + ToString(m_pMap->m_MapData[x][y][layer].m_uiAutoTileSetID) + ',');
//            xml_Line.AddAttribute("Tiles", sLine.c_str());
//            xml_Layer.AddChild("MapTiles", xml_Line);
//        }
//        xmlResult.AddChild("Layer", xml_Layer);
//    }
//}
//
//void MapWriter::_storeObjects(XML::XML_WriteData &xmlResult)
//{
//    for (uint32 layer = 0; layer < m_pMap->m_Objects.size(); ++layer)
//    {
//        XML::XML_WriteData xml_Layer(XML::XML_WRITE_APPEND);
//        //for (uint32 i = 0; i < m_pMap->m_Objects.at(layer).size(); ++i)
//        //{
//        //    XML::XML_WriteData xml_Object(XML::XML_WRITE_ADD);
//        //    //getXMLDataFromObject(m_pMap->m_Objects.at(layer).at(i), xml_Object);
//        //    xml_Layer.AddChild("Object", xml_Object);
//        //}
//        xmlResult.AddChild("Layer", xml_Layer);
//    }
//}
//
//void MapWriter::getXMLDataFromObject(const MapObjectPtr &obj, XML::XML_WriteData &xmlResult)
//{
//    xmlResult.AddAttribute("ID", obj->m_ObjectID);
//    xmlResult.AddAttribute("GUID", obj->m_GUID);
//    xmlResult.AddAttribute("XPos", obj->m_Position.x);
//    xmlResult.AddAttribute("YPos", obj->m_Position.y);
//    xmlResult.AddAttribute("Direction", obj->m_uiDirection);
//}

void MapWriter::storeMapThreaded(const QString &sFileName, bool &result)
{
    result = false;
    //if (!m_pMap || !m_pMap->hasMapDataStored())
    //    return;

    //XML::XML_WriteData xmlMap(XML::XML_WRITE_ADD);
    //XML::XML_WriteData xmlTiles(XML::XML_WRITE_OVERWRITE);
    //XML::XML_WriteData xmlObjects(XML::XML_WRITE_OVERWRITE);
    //boost::thread storeTiles(boost::bind(&MapWriter::_storeTiles, this, boost::ref(xmlTiles)));
    //boost::thread storeObjects(boost::bind(&MapWriter::_storeObjects, this, boost::ref(xmlObjects)));
    //XML::XML_WriteData xmlFile(XML::XML_WRITE_ADD);
    //storeTiles.join();
    //storeObjects.join();
    //xmlMap.AddChild("MapData", xmlTiles);
    //xmlMap.AddChild("ObjectData", xmlObjects);
    //xmlFile.AddChild("Map", xmlMap);
    //if (!boost::filesystem::exists(sFileName))
    //{
    //    std::ofstream file(sFileName.c_str());
    //    file << "<Map></Map>";
    //    file.close();
    //}
    //XMLWriter writer;
    //result = writer..writeFile(sFileName, xmlFile);
}