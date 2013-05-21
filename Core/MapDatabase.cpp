#include "MapDatabase.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include "MapIO.h"

using namespace MAP;
using namespace DATABASE;

MapObjectPtr MapPrototype::addMapObject(DATABASE::ObjectType type, uint32 uiID, Point3D<uint32> pos)
{
    MapObjectPtr newObject(new MapObject());
    newObject->m_ObjectID = uiID;
    newObject->m_Type = type;
    newObject->m_Position = pos;
    m_Objects.push_back(newObject);
    newObject->m_GUID = m_Objects.size();
    //m_Grid.addObject(newObject);
    return newObject;
}

void MapPrototype::moveMapObject(uint32 uiGUID, const Point3D<uint32> &newPos)
{
    if (uiGUID < m_Objects.size() && m_Objects.at(uiGUID))
    {
        //m_Grid.removeObject(m_Objects.at(uiGUID), MapGrid::getGridFromMapPos(m_Objects.at(uiGUID)->m_Position));
        m_Objects.at(uiGUID)->m_Position = newPos;
        //m_Grid.addObject(m_Objects.at(uiGUID));
    }
}

uint32 MapPrototype::getPositionsAroundWithID(const uint32 &uiID, const Point3D<uint32> &pos, UInt32PointSet &result, uint32 resultFlag)
{
    uint32 uiBorderCheck = 0;
    MapTile centerTile = getMapTile(pos);
    for (uint32 i = 0; i < 8; ++i)
    {
        DATABASE::AutoTilePrototype::TILE_CHECK curTileCheck = DATABASE::AutoTilePrototype::SAME_AROUND;
        // set position check
        Point3D<uint32> checkPos = pos;
        switch (i)
        {
        case 0: // top-left
            if (!checkPos.x || !checkPos.y)
                continue;
            --checkPos.x;
            --checkPos.y;
            curTileCheck = DATABASE::AutoTilePrototype::OTHER_TOP_LEFT;
            break;

        case 1: // top
            if (!checkPos.y)
                continue;
            --checkPos.y;
            curTileCheck = DATABASE::AutoTilePrototype::OTHER_TOP;
            break;

        case 2: // top-right
            if (!checkPos.y || checkPos.x+1 >= getSize().x)
                continue;
            ++checkPos.x;
            --checkPos.y;
            curTileCheck = DATABASE::AutoTilePrototype::OTHER_TOP_RIGHT;
            break;

        case 3: // left
            if (!checkPos.x)
                continue;
            --checkPos.x;
            curTileCheck = DATABASE::AutoTilePrototype::OTHER_LEFT;
            break;

        case 4: // right
            if (checkPos.x+1 >= getSize().x)
                continue;
            ++checkPos.x;
            curTileCheck = DATABASE::AutoTilePrototype::OTHER_RIGHT;
            break;

        case 5: // bottom-left
            if (!checkPos.x || checkPos.y+1 >= getSize().y)
                continue;
            --checkPos.x;
            ++checkPos.y;
            curTileCheck = DATABASE::AutoTilePrototype::OTHER_BOTTOM_LEFT;
            break;

        case 6: // bottom
            if (checkPos.y+1 >= getSize().y)
                continue;
            ++checkPos.y;
            curTileCheck = DATABASE::AutoTilePrototype::OTHER_BOTTOM;
            break;

        case 7: // bottom-right
            if (checkPos.x+1 >= getSize().x || checkPos.y+1 >= getSize().y)
                continue;
            ++checkPos.x;
            ++checkPos.y;
            curTileCheck = DATABASE::AutoTilePrototype::OTHER_BOTTOM_RIGHT;
            break;
        }
        MapTile mapTile = getMapTile(checkPos);
        // if bad object, continue
        if (mapTile.m_uiAutoTileSetID == MAX_UINT32 || mapTile.m_uiTileID == MAX_UINT32)
            continue;

        // if not same
        if (mapTile.m_uiAutoTileSetID != uiID)
        {
            uiBorderCheck += curTileCheck;
            if (resultFlag & FLAG_OTHER)
                result.insert(checkPos);
        }
        // if same
        else if (resultFlag & FLAG_SAME)
            result.insert(checkPos);
    }
    return uiBorderCheck;
}

void MapPrototype::_resizeMap(const Point3D<uint32> &size)
{
    m_MapData.resize(boost::extents[size.x][size.y][size.z]);
}

void MapPrototype::_clear()
{
    m_MapData.resize(boost::extents[0][0][0]);
}

void MapPrototype::setSize(const Point3D<uint32> &size)
{
    if (size != getSize())
    {
        if (hasMapDataStored())
            _resizeMap(size);
        m_Size = size;
    }
}

uint32 MapPrototype::getTile(const Point3D<uint32> &at) const
{
    if (!hasMapDataStored() || at.x >= m_Size.x || at.y >= m_Size.y || at.z >= m_Size.z)
        return MAX_UINT32;

    return m_MapData[at.x][at.y][at.z].m_uiTileID;
}

void MapPrototype::setTile(const Point3D<uint32> &at, uint32 uiID)
{
    if (!hasMapDataStored() || at.x >= m_Size.x || at.y >= m_Size.y || at.z >= m_Size.z)
        return;
    m_MapData[at.x][at.y][at.z].m_uiTileID = uiID;
}

uint32 MapPrototype::getAutoTile(const Point3D<uint32> &at) const
{
    if (!hasMapDataStored() || at.x >= m_Size.x || at.y >= m_Size.y || at.z >= m_Size.z)
        return MAX_UINT32;

    return m_MapData[at.x][at.y][at.z].m_uiAutoTileSetID;
}

void MapPrototype::setAutoTile(const Point3D<uint32> &at, uint32 uiID)
{
    if (!hasMapDataStored() || at.x >= m_Size.x || at.y >= m_Size.y || at.z >= m_Size.z)
        return;
    m_MapData[at.x][at.y][at.z].m_uiAutoTileSetID = uiID;
}

void MapPrototype::setMapTile(const Point3D<uint32> &at, const MapTile &mapTile)
{
    if (!hasMapDataStored() || at.x >= m_Size.x || at.y >= m_Size.y || at.z >= m_Size.z)
        return;
    m_MapData[at.x][at.y][at.z] = mapTile;
}

MapTile MapPrototype::getMapTile(const Point3D<uint32> &at) const
{
    if (!hasMapDataStored() || at.x >= m_Size.x || at.y >= m_Size.y || at.z >= m_Size.z)
        return MapTile(MAX_UINT32, MAX_UINT32);
    return m_MapData[at.x][at.y][at.z];
}

MapDatabase::MapDatabase(void) : Database()
{
}

void MapDatabase::clear()
{
    Database::clear();
    m_RemovedMaps.clear();
}

bool MapDatabase::hasMapDataStored(uint32 uiIndex) const
{
    ConstMapPrototypePtr pMap;
    if (getPrototype(uiIndex, pMap) && pMap && pMap->hasMapDataStored())
        return true;
    return false;
}

void MapDatabase::saveMapInfo(const MapPrototypePtr &map, const QString &path)
{
    //XML::XML_WriteData xmlMapDB(XML::XML_WRITE_ADD);
    //// get map
    //XML::XML_WriteData newMap(XML::XML_WRITE_OVERWRITE);
    //if (!getXMLDataFromMapInfo(map, newMap))
    //    return;

    //xmlMapDB.AddChild("Map", newMap);
    //XML::XML_WriteData xmlFile(XML::XML_WRITE_OVERWRITE);
    //xmlFile.AddChild("MapDatabase", xmlMapDB);

    //XML::XML_Writer writer;
    //QString sFilePath;
    //getFilePath(map, sFilePath, path);
    //writer.startWriting(sFilePath, xmlFile);
}

void MapDatabase::deleteRemovedMaps(const QString &path)
{
    for (uint32 i = 0; i < m_RemovedMaps.size(); ++i)
        _deleteRemovedMap(m_RemovedMaps.at(i), path);
    m_RemovedMaps.clear();
}

void MapDatabase::_deleteRemovedMap(const MapPrototypePtr &map, const QString &path)
{
    if (boost::filesystem::exists((path+ "/Maps/" + map->getFileName()).toStdString()))
        boost::filesystem::remove_all((path + "/Maps/" + map->getFileName()).toStdString());
}

bool MapDatabase::removeMap(uint32 uiID)
{
    MapPrototypePtr pMap;
    if (!getPrototype(uiID, pMap))
        return false;
    m_RemovedMaps.push_back(pMap);
    setPrototype(uiID, MapPrototypePtr());
    return true;
}

MapPrototypePtr MapDatabase::getNewMap()
{
    MapPrototypePtr pMap;
    uint32 i = 0;
    for (; i < getDBSize(); ++i)
    {
        if (!getPrototype(i+1, pMap) || !pMap)
            break;
    }
    // add new map
    pMap = MapPrototypePtr(new MapPrototype(i+1, "map" + QString::number(i+1) + ".xml"));
    setPrototype(i+1, pMap);
    return pMap;
}

bool MapDatabase::loadMapFile(uint32 uiMapID, const QString &path)
{
    MapPrototypePtr map;
    if (!getPrototype(uiMapID, map))
        return false;

    if (map->m_DataLoaded)
        return true;

    // if success, return
    MapReader mapLoader(map);
    if (mapLoader.readFile(path + "/Maps/" + map->getFileName(), "Map"))
        return true;

    // if no success, init with stored size
    map->m_DataLoaded = true;
    map->_resizeMap(map->getSize());
    return true;
}

void MapDatabase::unloadMapFile(uint32 uiMapID)
{
    MapPrototypePtr map;
    if (!getPrototype(uiMapID, map))
        return;

    map->m_DataLoaded = false;
    map->_resizeMap(Point3D<uint32>());
    map->m_Objects.clear();
}

bool MapDatabase::getFilePath(const MapPrototypePtr &map, QString &result, const QString path)
{
    result = "";
    if (!map)
        return false;

    result = path + "/Maps/" + map->m_sFileName;
    return true;
}

QString MapDatabase::getDefaultDBPath(const QString &path)
{
    return path + "/Game/MapDatabase.xml";
}