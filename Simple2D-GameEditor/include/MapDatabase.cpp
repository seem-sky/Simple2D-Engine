#include "MapDatabase.h"
#include "MapBinaryIO.h"

using namespace DATABASE;
using namespace DATABASE::MAP_STRUCTURE;

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

    map->m_DataLoaded = true;
    map->setSize(map->getSize(), map->getLayerSize(MAP::LAYER_FOREGROUND), map->getLayerSize(MAP::LAYER_BACKGROUND));

    try
    {
        INPUT::MapBinaryReader reader;
        reader.readFile(path + MAP_FOLDER, map);
    }
    catch (const std::ios::failure&) {}
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

void DATABASE::MAP_STRUCTURE::saveMapFile(ConstMapPrototypePtr pMap, const QString &path)
{
    OUTPUT::MapBinaryWriter writer;
    writer.writeFile(path+MAP_FOLDER, pMap);
}