#include "MapDatabase.h"
#include "MapIO.h"

using namespace DATABASE;
using namespace DATABASE::MAP;

MapPrototype::MapPrototype(uint32 uiID, const QString &sFileName) : DATABASE::Prototype(uiID), m_sFileName(sFileName), m_uiParentID(0),
m_DataLoaded(false)
{
    memset(&m_uiLayer, 0, sizeof(m_uiLayer));
}

void MapPrototype::addMapObject(MapObjectPtr pObject)
{
    if (pObject)
        m_Objects.setItem(pObject->m_GUID, pObject);
}

MapObjectPtr MapPrototype::addMapObject(DATABASE::ObjectType type, uint32 uiID, Int32Point pos)
{
    MapObjectPtr newObject(new MapObject());
    newObject->m_ObjectID = uiID;
    newObject->m_Type = type;
    newObject->m_Position = pos;
    newObject->m_GUID = m_Objects.getSize()+1;
    m_Objects.setItem(newObject->m_GUID, newObject);
    return newObject;
}

uint32 MapPrototype::checkAutoTiles(const uint32 &uiID, const UInt32Point3D &pos, UInt32PointSet &result, Layer layer, uint32 resultFlag)
{
    uint32 uiBorderCheck = 0;
    MapTile centerTile = getMapTile(pos, layer);
    for (uint32 i = 0; i < 8; ++i)
    {
        DATABASE::AUTO_TILE::TILE_CHECK curTileCheck = DATABASE::AUTO_TILE::SAME_AROUND;
        // set position check
        UInt32Point3D checkPos = pos;
        switch (i)
        {
        case 0: // top-left
            if (!checkPos.x || !checkPos.y)
                continue;
            --checkPos.x;
            --checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP_LEFT;
            break;

        case 1: // top
            if (!checkPos.y)
                continue;
            --checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP;
            break;

        case 2: // top-right
            if (!checkPos.y || checkPos.x+1 >= getSize().x)
                continue;
            ++checkPos.x;
            --checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP_RIGHT;
            break;

        case 3: // left
            if (!checkPos.x)
                continue;
            --checkPos.x;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_LEFT;
            break;

        case 4: // right
            if (checkPos.x+1 >= getSize().x)
                continue;
            ++checkPos.x;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_RIGHT;
            break;

        case 5: // bottom-left
            if (!checkPos.x || checkPos.y+1 >= getSize().y)
                continue;
            --checkPos.x;
            ++checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM_LEFT;
            break;

        case 6: // bottom
            if (checkPos.y+1 >= getSize().y)
                continue;
            ++checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM;
            break;

        case 7: // bottom-right
            if (checkPos.x+1 >= getSize().x || checkPos.y+1 >= getSize().y)
                continue;
            ++checkPos.x;
            ++checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM_RIGHT;
            break;
        }
        MapTile mapTile = getMapTile(checkPos, layer);
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

void MapPrototype::_resizeMap(UInt32Point size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize)
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

void MapPrototype::setSize(UInt32Point size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize)
{
    if (size != getSize() || m_uiLayer[LAYER_FOREGROUND] != uiForegroundLayerSize || m_uiLayer[LAYER_BACKGROUND] != uiBackgroundLayerSize)
    {
        if (hasMapDataStored())
            _resizeMap(size, uiForegroundLayerSize, uiBackgroundLayerSize);
        m_Size = size;
        m_uiLayer[LAYER_BACKGROUND] = uiBackgroundLayerSize;
        m_uiLayer[LAYER_FOREGROUND] = uiForegroundLayerSize;
    }
}

uint32 MapPrototype::getTile(UInt32Point3D at, Layer layer) const
{
    if (hasMapDataStored() && isInMap(at) && m_uiLayer[layer] > at.z)
    {
        switch (layer)
        {
        case LAYER_BACKGROUND: return m_BackgroundTiles[at.x][at.y][at.z].m_uiTileID;
        case LAYER_FOREGROUND: return m_ForegroundTiles[at.x][at.y][layer].m_uiTileID;
        }
    }
    return MAX_UINT32;
}

void MapPrototype::setTile(UInt32Point3D at, uint32 uiID, Layer layer)
{
    if (hasMapDataStored() && isInMap(at) && m_uiLayer[layer] > at.z)
    {
        switch (layer)
        {
        case LAYER_BACKGROUND: m_BackgroundTiles[at.x][at.y][at.z].m_uiTileID = uiID; break;
        case LAYER_FOREGROUND: m_ForegroundTiles[at.x][at.y][at.z].m_uiTileID = uiID; break;
        }            
    }
}

uint32 MapPrototype::getAutoTile(UInt32Point3D at, Layer layer) const
{
    if (hasMapDataStored() && isInMap(at) && m_uiLayer[layer] > at.z)
    {
        switch (layer)
        {
        case LAYER_BACKGROUND: return m_BackgroundTiles[at.x][at.y][at.z].m_uiAutoTileSetID;
        case LAYER_FOREGROUND: return m_ForegroundTiles[at.x][at.y][at.z].m_uiAutoTileSetID;
        }
    }
    return MAX_UINT32;
}

void MapPrototype::setAutoTile(UInt32Point3D at, uint32 uiID, Layer layer)
{
    if (hasMapDataStored() && isInMap(at) && m_uiLayer[layer] > at.z)
    {
        switch (layer)
        {
        case LAYER_BACKGROUND: m_BackgroundTiles[at.x][at.y][at.z].m_uiAutoTileSetID = uiID; break;
        case LAYER_FOREGROUND: m_ForegroundTiles[at.x][at.y][at.z].m_uiAutoTileSetID = uiID; break;
        }            
    }
}

void MapPrototype::setMapTile(UInt32Point3D at, MapTile mapTile, Layer layer)
{
    if (hasMapDataStored() && isInMap(at) && m_uiLayer[layer] > at.z)
    {
        switch (layer)
        {
        case LAYER_BACKGROUND: m_BackgroundTiles[at.x][at.y][at.z] = mapTile; break;
        case LAYER_FOREGROUND: m_ForegroundTiles[at.x][at.y][at.z] = mapTile; break;
        }            
    }
}

MapTile MapPrototype::getMapTile(UInt32Point3D at, Layer layer) const
{
    if (hasMapDataStored() && isInMap(at) && m_uiLayer[layer] > at.z)
    {
        switch (layer)
        {
        case LAYER_BACKGROUND: return m_BackgroundTiles[at.x][at.y][at.z];
        case LAYER_FOREGROUND: return m_ForegroundTiles[at.x][at.y][at.z];
        }
    }
    return MapTile(MAX_UINT32, MAX_UINT32);
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

bool MapDatabase::removeMap(uint32 uiID)
{
    MapPrototypePtr pMap;
    if (!getItem(uiID, pMap))
        return false;
    setItem(uiID, MapPrototypePtr());
    return true;
}

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