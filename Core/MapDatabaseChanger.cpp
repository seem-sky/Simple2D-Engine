#include "MapDatabaseChanger.h"
#include <QtCore/QFile>
#include "Config.h"

using namespace DATABASE;
using namespace DATABASE::MAP;

MapDatabaseChanger::MapDatabaseChanger(MapDatabasePtr pMapDB) : DatabaseChanger(pMapDB)
{
    m_pNewDB = MapDatabasePtr(new MapDatabase());
}

MapDatabaseChanger::MapDatabaseChanger() : DatabaseChanger()
{
    m_pNewDB = MapDatabasePtr(new MapDatabase());
}

void MapDatabaseChanger::undoChanges(uint32 uiID)
{
    // unload map data and reset size
    _getNewDB()->unloadMapFile(uiID);
    MapPrototypePtr pNewMap;
    MapPrototypePtr pTargetMap;
    if (_getTargetDB()->getItem(uiID, pTargetMap) && _getNewDB()->getItem(uiID, pNewMap))
        pNewMap->setSize(pTargetMap->getSize(), pTargetMap->getLayerSize(LAYER_FOREGROUND), pTargetMap->getLayerSize(LAYER_BACKGROUND));
}

void MapDatabaseChanger::storeChanges()
{
    if (!m_pTargetDB)
        return;
    // delete removed maps
    for (UInt32MapPrototypeMap::const_iterator itr = m_removedMaps.begin(); itr != m_removedMaps.end(); ++itr)
    {
        _getTargetDB()->removeMap(itr->first);
        if (itr->second)
        {
            QFile file(Config::Get()->getProjectDirectory() + "/Maps/" + itr->second->getFileName());
            file.remove();
        }
    }
    m_removedMaps.clear();

    DatabaseChanger::storeChanges();
}

bool MapDatabaseChanger::removeMap(uint32 uiID)
{
    MapPrototypePtr proto;
    if (getItem(uiID, proto) && _getTargetDB()->removeMap(uiID))
    {
        m_removedMaps.insert(std::make_pair(uiID, proto));
        return true;
    }
    return false;
}

MapPrototypePtr MapDatabaseChanger::getNewMap()
{
    uint32 i = 1;
    for (; i < MAX_UINT32; ++i)
    {
        if (!m_pNewDB->getItem(i, MapPrototypePtr()) && !m_pTargetDB->getItem(i, MapPrototypePtr()))
        {
            // if not in m_removedMaps
            UInt32MapPrototypeMap::iterator itr = m_removedMaps.find(i);
            if (itr == m_removedMaps.end())
                break;
        }
    }
    // add new map
    MapPrototypePtr pMap(new MapPrototype(i, "map" + QString::number(i) + ".xml"));
    setItem(i, pMap);
    return pMap;
}

bool MapDatabaseChanger::loadMapFile(uint32 mapID, const QString &projectDir)
{
    return getItem(mapID, MapPrototypePtr()) && _getNewDB()->loadMapFile(mapID, projectDir);
}

void MapDatabaseChanger::unloadMapFile(uint32 mapID)
{
    _getNewDB()->unloadMapFile(mapID);
}