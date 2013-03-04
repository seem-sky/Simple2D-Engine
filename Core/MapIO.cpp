#include "MapIO.h"
#include <atlbase.h>
#include <msxml.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include "StringAdditions.h"

#import <msxml6.dll>

using namespace MAP;

/*#####
# MapReader
#####*/
void MapReader::loadMapThreaded(const std::string &sFileName, bool &result)
{
    result = false;
    if (STATE_NONE != m_State)
        return;

    m_State = STATE_IN_PROGRESS;
    bool tileResult = false, objectResult = false;
    boost::thread loadTiles(boost::bind(&MapReader::_loadTiles, this, boost::ref(tileResult), boost::cref(sFileName), true));
    boost::thread loadObjects(boost::bind(&MapReader::_loadObjects, this, boost::ref(objectResult), boost::cref(sFileName), true));
    loadTiles.join();
    loadObjects.join();
    m_State = STATE_DONE;
    if (tileResult && objectResult)
        result = true;
}

void MapReader::_loadTiles(bool &result, const std::string &sFileName, bool threaded)
{
    result = false;
    if (!m_pMap)
        return;

    // resize map data
    m_pMap->m_MapData.resize(boost::extents[0][0][0]);
    m_pMap->m_MapData.resize(boost::extents[m_pMap->m_Size.x][m_pMap->m_Size.y][m_pMap->m_Size.z]);

    XML::CoObject CoObj;
    if (threaded)
        CoObj.InitCo();
    StdString2DVector TileByRowFromFileList;
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    if (FAILED(pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument40))))
        return;

    if (!pXMLDom->load(_variant_t(sFileName.c_str())))
        return;

    IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
    if (!pNode)
        return;

    // get tile map from file
    pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("MapData");
    if (!pNode)
        return;

    IXMLDOMNodeListPtr pLayerNodes;
    if (FAILED(pNode->get_childNodes(&pLayerNodes)))
        return;

    // iterate through layers
    LONG iLayerLength = 0;
    if (FAILED(pLayerNodes->get_length(&iLayerLength)))
        return;

    IXMLDOMNodePtr pLayer = NULL;
    for (LONG j = 0; j < iLayerLength; j++)
    {
        if (FAILED(pLayerNodes->get_item(j, &pLayer)))
            continue;

        // get tiles
        IXMLDOMNodeListPtr pMapNodes;
        if (FAILED(pLayer->get_childNodes(&pMapNodes)))
            continue;

        LONG iNodeLength = 0;
        if (FAILED(pMapNodes->get_length(&iNodeLength)))
            continue;
        IXMLDOMNamedNodeMapPtr pmAttributes;

        CComBSTR attributeText;

        // iterate through tiles
        IXMLDOMNodePtr pNodeTemp = NULL;
        StdStringVector tempList;
        for (int i = 0; i < iNodeLength; i++)
        {
            // continue if there are no "Tiles" set
            if (FAILED(pMapNodes->get_item(i, &pNodeTemp)))
                continue;

            if (FAILED(pNodeTemp->get_attributes(&pmAttributes)))
                continue;

            if (FAILED(pmAttributes->get_item(0, &pNodeTemp)))
                continue;

            if (FAILED(pNodeTemp->get_text(&attributeText)))
                continue;

            std::string temp = _bstr_t(attributeText);
            tempList.push_back(temp);
        }
        TileByRowFromFileList.push_back(tempList);
    }

    // read out layers
    uint32 uiLayerCount = TileByRowFromFileList.size();
    for (uint32 layer = 0; layer < uiLayerCount && layer < m_pMap->getSize().z; layer++)
    {
        uint32 uiYCount = TileByRowFromFileList.at(layer).size();
        for (uint32 y = 0; y < uiYCount && y < m_pMap->getSize().y; ++y)
        {
            // if tiles are in string, read it out and store as readable vector.
            UInt32Vector tempVector;
            for (uint32 x = 0; x < m_pMap->getSize().x && !TileByRowFromFileList.at(layer).at(y).empty(); ++x)
            {
                bool tileStored = false;
                while (!tileStored && !TileByRowFromFileList.at(layer).at(y).empty())
                {
                    // if first sign is a number
                    if (TileByRowFromFileList.at(layer).at(y).at(0) <= 57 && TileByRowFromFileList.at(layer).at(y).at(0) >= 48)
                    {
                        m_pMap->m_MapData[x][y][layer] = (uint32)atoi(TileByRowFromFileList.at(layer).at(y).c_str());
                        TileByRowFromFileList.at(layer).at(y).erase(0, TileByRowFromFileList.at(layer).at(y).find(','));
                        tileStored = true;
                    }
                    // if first sign is a not a number, delete it
                    else
                        TileByRowFromFileList.at(layer).at(y).erase(0, 1);
                }
            }
        }
    }
    result = true;
}

void MapReader::_loadObjects(bool &result, const std::string &sFileName, bool threaded)
{
    result = false;
    if (!m_pMap)
        return;

    m_pMap->m_Objects.clear();
    XML::CoObject CoObj;
    if (threaded)
        CoObj.InitCo();
    MSXML2::IXMLDOMDocument2Ptr pXMLDom = NULL;
    if (FAILED(pXMLDom.CreateInstance(__uuidof(MSXML2::DOMDocument40))))
        return;

    if (!pXMLDom->load(_variant_t(sFileName.c_str())))
        return;

    IXMLDOMNodePtr pNode = pXMLDom->selectSingleNode("Map");
    if (!pNode)
        return;

    pNode = pXMLDom->selectSingleNode("Map")->selectSingleNode("ObjectData");
    if (!pNode)
        return;

    // read out layers
    IXMLDOMNodeListPtr pLayerNodeList;
    if (FAILED(pNode->get_childNodes(&pLayerNodeList)))
        return;

    LONG iLayerNodeLength = 0;
    if (FAILED(pLayerNodeList->get_length(&iLayerNodeLength)))
        return;

    IXMLDOMNodePtr pLayerNode = NULL;
    IXMLDOMNodeListPtr pObjectNodeList;
    for (LONG i = 0; i < iLayerNodeLength; i++)
    {
        // select layer
        if (FAILED(pLayerNodeList->get_item(i, &pLayerNode)))
            continue;

        // read out objects of layer
        if (FAILED(pLayerNode->get_childNodes(&pObjectNodeList)))
            continue;

        // iterate through objects
        LONG iObjectNodeLength = 0;
        if (FAILED(pObjectNodeList->get_length(&iObjectNodeLength)))
            continue;

        IXMLDOMNodePtr pObjectNode = NULL;
        MapObjectPtrVector vObjects;
        IXMLDOMNamedNodeMapPtr pmObjectAttributes;
        for (LONG j = 0; j < iObjectNodeLength; j++)
        {
            // select object
            if (FAILED(pObjectNodeList->get_item(j, &pObjectNode)))
                continue;

            // read out object attributes
            if (FAILED(pObjectNode->get_attributes(&pmObjectAttributes)))
                continue;

            LONG iAttributeLength = 0;
            if (FAILED(pmObjectAttributes->get_length(&iAttributeLength)))
                continue;
            IXMLDOMNodePtr pSelectedAttribute = NULL;
            CComVariant value;
            CComBSTR sNodeName;
            MapObjectPtr newObject(new MapObject());
            for (LONG k = 0; k < iAttributeLength; k++)
            {
                // select layer
                if (FAILED(pmObjectAttributes->get_item(k, &pSelectedAttribute)))
                    continue;

                // get node value
                if (FAILED(pSelectedAttribute->get_nodeValue(&value)))
                    continue;

                // get node name
                if (FAILED(pSelectedAttribute->get_nodeName(&sNodeName)))
                    continue;

                std::string sNode = _bstr_t(sNodeName);
                if (FAILED(value.ChangeType(VT_UINT)))
                    continue;
                if (sNode == "ID")
                    newObject->m_ObjectID = value.uintVal;
                else if (sNode == "GUID")
                    newObject->m_GUID = value.uintVal;
                else if (sNode == "XPos")
                    newObject->m_Position.x = value.uintVal;
                else if (sNode == "YPos")
                    newObject->m_Position.y = value.uintVal;
                else if (sNode == "Direction")
                    newObject->m_uiDirection = value.uintVal;
            }
            vObjects.push_back(newObject);
        }
        m_pMap->m_Objects.push_back(vObjects);
    }
    result = true;
}

/*#####
# MapWriter
#####*/
void MapWriter::_storeTiles(XML::XML_WriteData &xmlResult)
{
    for (uint32 layer = 0; layer < m_pMap->getSize().z; ++layer)
    {
        XML::XML_WriteData xml_Layer(XML::XML_WRITE_APPEND);
        for (uint32 y = 0; y < m_pMap->getSize().y; ++y)
        {
            XML::XML_WriteData xml_Line(XML::XML_WRITE_APPEND);
            std::string sLine;
            for (uint32 x = 0; x < m_pMap->getSize().x; ++x)
                sLine.append(ToString(m_pMap->m_MapData[x][y][layer]) + ',');
            xml_Line.AddAttribute("Tiles", sLine.c_str());
            xml_Layer.AddChild("MapTiles", xml_Line);
        }
        xmlResult.AddChild("Layer", xml_Layer);
    }
}

void MapWriter::_storeObjects(XML::XML_WriteData &xmlResult)
{
    for (uint32 layer = 0; layer < m_pMap->m_Objects.size(); ++layer)
    {
        XML::XML_WriteData xml_Layer(XML::XML_WRITE_APPEND);
        for (uint32 i = 0; i < m_pMap->m_Objects.at(layer).size(); ++i)
        {
            XML::XML_WriteData xml_Object(XML::XML_WRITE_ADD);
            getXMLDataFromObject(m_pMap->m_Objects.at(layer).at(i), xml_Object);
            xml_Layer.AddChild("Object", xml_Object);
        }
        xmlResult.AddChild("Layer", xml_Layer);
    }
}

void MapWriter::getXMLDataFromObject(const MapObjectPtr &obj, XML::XML_WriteData &xmlResult)
{
    xmlResult.AddAttribute("ID", obj->m_ObjectID);
    xmlResult.AddAttribute("GUID", obj->m_GUID);
    xmlResult.AddAttribute("XPos", obj->m_Position.x);
    xmlResult.AddAttribute("YPos", obj->m_Position.y);
    xmlResult.AddAttribute("Direction", obj->m_uiDirection);
}

void MapWriter::storeMapThreaded(const std::string &sFileName, bool &result)
{
    result = false;
    if (!m_pMap || !m_pMap->hasMapDataStored())
        return;

    XML::XML_WriteData xmlMap(XML::XML_WRITE_ADD);
    XML::XML_WriteData xmlTiles(XML::XML_WRITE_OVERWRITE);
    XML::XML_WriteData xmlObjects(XML::XML_WRITE_OVERWRITE);
    boost::thread storeTiles(boost::bind(&MapWriter::_storeTiles, this, boost::ref(xmlTiles)));
    boost::thread storeObjects(boost::bind(&MapWriter::_storeObjects, this, boost::ref(xmlObjects)));
    XML::XML_WriteData xmlFile(XML::XML_WRITE_ADD);
    storeTiles.join();
    storeObjects.join();
    xmlMap.AddChild("MapData", xmlTiles);
    xmlMap.AddChild("ObjectData", xmlObjects);
    xmlFile.AddChild("Map", xmlMap);
    if (!boost::filesystem::exists(sFileName))
    {
        std::ofstream file(sFileName.c_str());
        file << "<Map></Map>";
        file.close();
    }
    XML::XML_Writer writer;
    result = writer.startWriting(sFileName, xmlFile);
}