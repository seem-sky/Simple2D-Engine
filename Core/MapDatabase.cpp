#include "MapDatabase.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include "MapIO.h"

using namespace MAP;

uint32 MapPrototype::getTile(Point3D<uint32> at) const
{
    if (!hasMapDataStored() || at.x >= m_Size.x || at.y >= m_Size.y || at.z >= m_Size.z)
        return MAX_UINT32;

    return m_MapData[at.x][at.y][at.z];
}

void MapPrototype::setTile(Point3D<uint32> at, uint32 uiID)
{
    if (!hasMapDataStored() || at.x >= m_Size.x || at.y >= m_Size.y || at.z >= m_Size.z)
        return;
    m_MapData[at.x][at.y][at.z] = uiID;
}

MapDatabase::MapDatabase(void) : TSingleton()
{
}

void MapDatabase::unloadMapDatabase()
{
    m_pMaps.clear();
    m_pRemovedMaps.clear();
}

bool MapDatabase::hasMapDataStored(uint32 uiIndex) const
{
    MapPrototypePtrMap::const_iterator itr = m_pMaps.find(uiIndex);
    if (itr != m_pMaps.end() && itr->second)
        return itr->second->hasMapDataStored();
    return false;
}

void MapDatabase::loadMapDatabase(bool &result, const std::string &sPath)
{
    result = false;
    m_pMaps.clear();
    XML::XML_Reader reader(getDefaultDBPath(sPath).c_str());
    if (!reader.ReadFile())
        return;

    XML::ReadChildList childList = reader.GetXMLData();
    XML::ReadChildList::iterator mapDBitr = childList.find("MapDatabase");
    if (mapDBitr == childList.end())
        return;

    MapPrototypePtr newMap;
    for (XML::ReadChildList::const_iterator itr = mapDBitr->second.GetChildList()->find("Map"); itr != mapDBitr->second.GetChildList()->end() &&
        itr->first == "Map"; ++itr)
    {
        newMap = MapPrototypePtr(new MapPrototype());
        // get map attributes
        getMapInfoFromXMLData(itr->second, newMap);
        m_pMaps.insert(std::make_pair(newMap->m_uiID, newMap));
    }

    result = true;
}

void MapDatabase::saveMapInfo(const MapPrototypePtr &map, const std::string &path)
{
    XML::XML_WriteData xmlMapDB(XML::XML_WRITE_ADD);
    // get map
    XML::XML_WriteData newMap(XML::XML_WRITE_OVERWRITE);
    MapPrototypePtrMap::const_iterator itr = m_pMaps.find(map->getID());
    if (itr == m_pMaps.end() || !getXMLDataFromMapInfo(itr->second, newMap))
        return;

    xmlMapDB.AddChild("Map", newMap);
    XML::XML_WriteData xmlFile(XML::XML_WRITE_OVERWRITE);
    xmlFile.AddChild("MapDatabase", xmlMapDB);

    XML::XML_Writer writer;
    std::string sFilePath;
    getFilePath(map, sFilePath, path);
    writer.startWriting(sFilePath, xmlFile);
}

void MapDatabase::saveMapDatabase(bool &result, const std::string &path)
{
    result = false;
    XML::XML_WriteData xmlMapDB(XML::XML_WRITE_ADD);
    // get maps
    for (MapPrototypePtrMap::const_iterator itr = m_pMaps.begin(); itr != m_pMaps.end(); ++itr)
    {
        XML::XML_WriteData newMap(XML::XML_WRITE_ADD);
        if (!getXMLDataFromMapInfo(itr->second, newMap))
            continue;

        xmlMapDB.AddChild("Map", newMap);
    }

    // get removed maps
    for (MapPrototypePtrMap::const_iterator itr = m_pRemovedMaps.begin(); itr != m_pRemovedMaps.end(); ++itr)
    {
        XML::XML_WriteData removedMap(XML::XML_WRITE_DELETE);
        if (!getXMLDataFromMapInfo(itr->second, removedMap))
            continue;

        _deleteRemovedMap(itr->second, path);
        xmlMapDB.AddChild("Map", removedMap);
    }
    m_pRemovedMaps.clear();

    XML::XML_WriteData xmlFile(XML::XML_WRITE_OVERWRITE);
    xmlFile.AddChild("MapDatabase", xmlMapDB);

    XML::XML_Writer writer;
    if (writer.startWriting(getDefaultDBPath(path), xmlFile))
        result = true;
}

void MapDatabase::_deleteRemovedMap(const MapPrototypePtr &map, const std::string &path)
{
    if (boost::filesystem::exists(path+ "/Maps/" + map->getFileName()))
        boost::filesystem::remove_all(path + "/Maps/" + map->getFileName());
}

bool MapDatabase::getXMLDataFromMapInfo(const MapPrototypePtr &map, XML::XML_WriteData &result)
{
    if (!map)
        return false;

    result.AddAttribute("File", map->getFileName().c_str());
    result.AddAttribute("MapAnnounceName", map->getAnnounceName().c_str());
    result.AddAttribute("MapScript", map->getScriptName().c_str());
    result.AddAttribute("ID", map->getID());
    result.AddAttribute("ParentID", map->getParentID());
    result.AddAttribute("MapSizeX", map->getSize().x);
    result.AddAttribute("MapSizeY", map->getSize().y);
    result.AddAttribute("Layer", map->getSize().z);
    return true;
}

void MapDatabase::getMapInfoFromXMLData(const XML::XML_ReadData &data, MapPrototypePtr &result)
{
    for (XML::AttributeMap::const_iterator attrItr = data.GetAttributeList()->begin(); attrItr != data.GetAttributeList()->end(); ++attrItr)
    {
        if ("File" == attrItr->first)
        {
            result->m_sFileName = _bstr_t(attrItr->second.bstrVal);
            continue;
        }
        else if ("MapAnnounceName" == attrItr->first)
        {
            result->m_sAnnounceName = _bstr_t(attrItr->second.bstrVal);
            continue;
        }
        else if ("MapScript" == attrItr->first)
        {
            result->m_sScriptName = _bstr_t(attrItr->second.bstrVal);
            continue;
        }

        CComVariant value = attrItr->second;
        if (FAILED(value.ChangeType(VT_UINT)))
            continue;
        if ("ID" == attrItr->first)
            result->m_uiID = value.uintVal;
        if ("ParentID" == attrItr->first)
            result->m_uiParentID = value.uintVal;
        if ("MapSizeX" == attrItr->first)
            result->m_Size.x = value.uintVal;
        if ("MapSizeY" == attrItr->first)
            result->m_Size.y = value.uintVal;
        if ("Layer" == attrItr->first)
            result->m_Size.z = value.uintVal;
    }

    if (result->m_uiID == result->m_uiParentID)
        result->m_uiParentID = 0;
}

bool MapDatabase::removeMap(uint32 uiID)
{
    MapPrototypePtrMap::iterator itr = m_pMaps.find(uiID);
    if (itr != m_pMaps.end())
    {
        m_pRemovedMaps.insert(std::make_pair(itr->first, itr->second));
        m_pMaps.erase(itr);
        return true;
    }

    return false;
}

void MapDatabase::_resizeMap(const MapPrototypePtr &map, Point3D<uint32> newSize) const
{
    if (!map)
        return;
    if (map->m_DataLoaded)
        map->m_MapData.resize(boost::extents[newSize.x][newSize.y][newSize.z]);
    map->m_Size = newSize;
}

void MapDatabase::setMapInfo(uint32 uiID, const std::string &sMapName, const std::string &sAnnounceName, const std::string &sScriptName, const Point3D<uint32> &size)
{
    if (!uiID)
        return;

    MapPrototypePtrMap::iterator itr = m_pMaps.find(uiID);
    if (itr != m_pMaps.end())
    {
        itr->second->m_sFileName = sMapName;
        itr->second->m_sAnnounceName = sAnnounceName;
        itr->second->m_sScriptName = sScriptName;
        if (itr->second->m_Size != size)
            _resizeMap(itr->second, size);
    }
    else
    {
        MapPrototypePtr newMap(new MapPrototype(uiID, sMapName));
        newMap->m_sAnnounceName = sAnnounceName;
        newMap->m_sScriptName = sScriptName;
        newMap->m_Size = size;
        m_pMaps.insert(std::make_pair(uiID, newMap));
    }
}

uint32 MapDatabase::getFreeID() const
{
    if (m_pMaps.empty())
        return 1;

    MapPrototypePtrMap::const_iterator itr = --(m_pMaps.end());
    return itr->first+1;
}

bool MapDatabase::getMap(uint32 uiID, MapPrototypePtr &result) const
{
    MapPrototypePtrMap::const_iterator itr = m_pMaps.find(uiID);
    if (itr != m_pMaps.end())
    {
        result = itr->second;
        return true;
    }

    return false;
}

bool MapDatabase::loadMapFile(uint32 uiMapID, const std::string &sPath)
{
    MapPrototypePtr map;
    if (!getMap(uiMapID, map))
        return false;

    if (map->m_DataLoaded)
        return true;

    std::string sFilePath;
    if (getFilePath(map, sFilePath, sPath) && boost::filesystem::exists(sFilePath))
    {
        bool loadResult = false;
        MapReader mapLoader(map);
        mapLoader.loadMapThreaded(sFilePath, loadResult);
        map->m_DataLoaded = true;
        if (loadResult)
            return true;
    }

    map->m_DataLoaded = true;
    _resizeMap(map, map->getSize());
    return true;
}

void MapDatabase::unloadMapFile(uint32 uiMapID)
{
    MapPrototypePtr map;
    if (!getMap(uiMapID, map))
        return;

    map->m_DataLoaded = false;
    map->m_MapData.resize(boost::extents[0][0][0]);
    map->m_Objects.clear();
}

bool MapDatabase::getFilePath(const MapPrototypePtr &map, std::string &result, const std::string path)
{
    result = "";
    if (!map)
        return false;

    result = path + "/Maps/" + map->m_sFileName;
    return true;
}

std::string MapDatabase::getDefaultDBPath(const std::string &path)
{
    return path + "/Game/MapDatabase.xml";
}