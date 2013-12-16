#include "MapDatabase.h"
#include "MapBinaryIO.h"

using namespace DATABASE;
using namespace DATABASE::MAP_STRUCTURE;

void MapDatabase::clear()
{
    Database::clear();
    m_RemovedMaps.clear();
}

bool MapDatabase::hasMapDataStored(uint32 uiIndex) const
{
    auto pMap = getOriginalPrototype(uiIndex);
    if (pMap && pMap->hasMapDataStored())
        return true;
    return false;
}

bool MapDatabase::removeMap(uint32 uiID)
{
    if (!getOriginalPrototype(uiID))
        return false;
    // ToDo: setPrototype(uiID, nullptr);
    return true;
}

bool MapDatabase::loadMapFile(uint32 uiMapID, const QString &path)
{
    auto pMap = getOriginalPrototype(uiMapID);
    if (!pMap)
        return false;

    if (pMap->m_DataLoaded)
        return true;

    pMap->m_DataLoaded = true;
    pMap->setSize(pMap->getSize(), pMap->getLayerSize(MAP::LAYER_FOREGROUND), pMap->getLayerSize(MAP::LAYER_BACKGROUND));

    try
    {
        INPUT::MapBinaryReader reader;
        reader.readFile(path + MAP_FOLDER, pMap);
    }
    catch (const std::ios::failure&) {}
    return true;
}

void MapDatabase::unloadMapFile(uint32 uiMapID)
{
    auto pMap = getOriginalPrototype(uiMapID);
    if (!pMap)
        return;

    pMap->_clearTiles();
    pMap->m_Objects.clear();
}

MapPrototypePtr MapDatabase::getNewPrototype()
{
    // search for unvalid prototypes
    for (auto &pPrototype : getPrototypes())
    {
        if (!pPrototype->isValid())
            return MapPrototypePtr(new MapPrototype(*pPrototype));
    }

    return MapPrototypePtr(new MapPrototype(getSize()+1));
}

//void MapDatabase::setPrototype(MapPrototype *pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const MapPrototype* MapDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const MapPrototype*>(Database::getPrototype(ID));
//}
//
//MapPrototype* MapDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<MapPrototype*>(Database::getPrototype(ID));
//}
//
//MapPrototype* MapDatabase::getNewPrototype(uint32 ID) const
//{
//    return new MapPrototype(ID);
//}

void DATABASE::MAP_STRUCTURE::saveMapFile(const MapPrototype *pMap, const QString &path)
{
    OUTPUT::MapBinaryWriter writer;
    writer.writeFile(path+MAP_FOLDER, pMap);
}