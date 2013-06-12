#include "MapDatabase.h"
#include "MapIO.h"

using namespace MAP;
using namespace DATABASE;

void MapPrototype::addMapObject(MapObjectPtr pObject)
{
    if (pObject)
        m_Objects.push_back(pObject);
}

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

void MapPrototype::_resizeMap(Point<uint32> size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize)
{
    m_BackgroundTiles.resize(boost::extents[size.x][size.y][uiBackgroundLayerSize]);
    m_ForegroundTiles.resize(boost::extents[size.x][size.y][uiForegroundLayerSize]);
}

void MapPrototype::_clearTiles()
{
    m_DataLoaded = false;
    m_BackgroundTiles.resize(boost::extents[0][0][0]);
    m_ForegroundTiles.resize(boost::extents[0][0][0]);
}

void MapPrototype::setSize(Point<uint32> size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize)
{
    if (size != getSize() || uiForegroundLayerSize != m_uiForegroundLayer || uiBackgroundLayerSize != m_uiBackgroundLayer)
    {
        if (hasMapDataStored())
            _resizeMap(size, uiForegroundLayerSize, uiBackgroundLayerSize);
        m_Size = size;
    }
}

uint32 MapPrototype::getTile(Point3D<uint32> at, Layer layer) const
{
    if (hasMapDataStored() && isInMap(at))
    {
        if (layer == LAYER_BACKGROUND && m_uiBackgroundLayer > at.z)
            return m_BackgroundTiles[at.x][at.y][layer].m_uiTileID;
        else if (layer == LAYER_FOREGROUND && m_uiForegroundLayer > at.z)
            return m_ForegroundTiles[at.x][at.y][layer].m_uiTileID;
    }
    return MAX_UINT32;
}

void MapPrototype::setTile(Point3D<uint32> at, uint32 uiID, Layer layer)
{
    if (hasMapDataStored() && isInMap(at))
    {
        if (layer == LAYER_BACKGROUND && m_uiBackgroundLayer > at.z)
            m_BackgroundTiles[at.x][at.y][at.z].m_uiTileID = uiID;
        else if (layer == LAYER_FOREGROUND && m_uiForegroundLayer > at.z)
            m_ForegroundTiles[at.x][at.y][at.z].m_uiTileID = uiID;
    }
}

uint32 MapPrototype::getAutoTile(Point3D<uint32> at, Layer layer) const
{
    if (hasMapDataStored() && isInMap(at))
    {
        if (layer == LAYER_BACKGROUND && m_uiBackgroundLayer > at.z)
            return m_BackgroundTiles[at.x][at.y][layer].m_uiAutoTileSetID;
        else if (layer == LAYER_FOREGROUND && m_uiForegroundLayer > at.z)
            return m_ForegroundTiles[at.x][at.y][layer].m_uiAutoTileSetID;
    }
    return MAX_UINT32;
}

void MapPrototype::setAutoTile(Point3D<uint32> at, uint32 uiID, Layer layer)
{
    if (hasMapDataStored() && isInMap(at))
    {
        if (layer == LAYER_BACKGROUND && m_uiBackgroundLayer > at.z)
            m_BackgroundTiles[at.x][at.y][at.z].m_uiAutoTileSetID = uiID;
        else if (layer == LAYER_FOREGROUND && m_uiForegroundLayer > at.z)
            m_ForegroundTiles[at.x][at.y][at.z].m_uiAutoTileSetID = uiID;
    }
}

void MapPrototype::setMapTile(Point3D<uint32> at, MapTile mapTile, Layer layer)
{
    if (hasMapDataStored() && isInMap(at))
    {
        if (layer == LAYER_BACKGROUND && m_uiBackgroundLayer > at.z)
            m_BackgroundTiles[at.x][at.y][at.z] = mapTile;
        else if (layer == LAYER_FOREGROUND && m_uiForegroundLayer > at.z)
            m_ForegroundTiles[at.x][at.y][at.z] = mapTile;
    }
}

MapTile MapPrototype::getMapTile(Point3D<uint32> at, Layer layer) const
{
    if (hasMapDataStored() && isInMap(at))
    {
        if (layer == LAYER_BACKGROUND && m_uiBackgroundLayer > at.z)
            return m_BackgroundTiles[at.x][at.y][at.z];
        else if (layer == LAYER_FOREGROUND && m_uiForegroundLayer > at.z)
            return m_ForegroundTiles[at.x][at.y][at.z];
    }
    return MAX_UINT32;
}

uint32 MapPrototype::getLayerSize(Layer layer) const
{
    if (layer == LAYER_BACKGROUND)
        return m_uiBackgroundLayer;
    return m_uiForegroundLayer;
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
    if (getItem(uiIndex, pMap) && pMap && pMap->hasMapDataStored())
        return true;
    return false;
}

//void MapDatabase::deleteRemovedMaps(const QString &path)
//{
//    for (uint32 i = 0; i < m_RemovedMaps.size(); ++i)
//        _deleteRemovedMap(m_RemovedMaps.at(i), path);
//    m_RemovedMaps.clear();
//}
//
//void MapDatabase::_deleteRemovedMap(const MapPrototypePtr &map, const QString &path)
//{
//    if (map)
//    {
//        QFile file(path+ "/Maps/" + map->getFileName());
//        file.remove();
//    }
//}

bool MapDatabase::removeMap(uint32 uiID)
{
    MapPrototypePtr pMap;
    if (!getItem(uiID, pMap))
        return false;
    setItem(uiID, MapPrototypePtr());
    return true;
}

//MapPrototypePtr MapDatabase::getNewMap()
//{
//    MapPrototypePtr pMap;
//    uint32 i = 0;
//    for (; i < getSize(); ++i)
//    {
//        if (!getItem(i+1, pMap) || !pMap)
//            break;
//    }
//    // add new map
//    pMap = MapPrototypePtr(new MapPrototype(i+1, "map" + QString::number(i+1) + ".xml"));
//    setItem(i+1, pMap);
//    return pMap;
//}

bool MapDatabase::loadMapFile(uint32 uiMapID, const QString &path)
{
    MapPrototypePtr map;
    if (!getItem(uiMapID, map))
        return false;

    if (map->m_DataLoaded)
        return true;

    // if success, return
    MapReader mapLoader(map);
    mapLoader.readFile(path + "/Maps/" + map->getFileName(), "Map");
    if (mapLoader.getXMLResult() != XML_IO::DONE)
    {
        // if no success, init with stored size
        map->m_DataLoaded = true;
        map->_resizeMap(map->getSize(), map->getLayerSize(LAYER_FOREGROUND), map->getLayerSize(LAYER_BACKGROUND));
    }
    return true;
}

void MapDatabase::unloadMapFile(uint32 uiMapID)
{
    MapPrototypePtr map;
    if (!getItem(uiMapID, map))
        return;

    map->_clearTiles();
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