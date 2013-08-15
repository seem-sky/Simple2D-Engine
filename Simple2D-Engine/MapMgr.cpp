#include "MapMgr.h"

using namespace MAP;

MapMgr::MapMgr() : SpaceContainer()
{}

void MapMgr::setDatabaseMgr(DATABASE::ConstDatabaseMgrPtr pDBMgr)
{
    m_pDBMgr = pDBMgr;
}

MapPtr MapMgr::loadMap(uint32 uiID)
{
    MapPtr pMap;
    DATABASE::MAP_STRUCTURE::MapPrototypePtr pMapProto;
    DATABASE::MapDatabasePtr pMapDB = std::const_pointer_cast<DATABASE::MAP_STRUCTURE::MapDatabase>(m_pDBMgr->getMapDatabase());
    if (pMapDB && pMapDB->loadMapFile(uiID, "projects/untitled/") && pMapDB->getItem(uiID, pMapProto))
    {
        pMap = MapPtr(new Map(getSize()+1, pMapProto->getID(), pMapProto->m_Layer, pMapProto->getMapObjects(), m_pDBMgr, pMapProto->getName().toStdString(), pMapProto->getScriptName().toStdString()));
        m_Maps.push_back(pMap);
        setItem(pMap->getGUID(), pMap);
    }
    return std::move(pMap);
}

void MapMgr::updateMaps(uint32 uiDiff)
{
    for (auto x : m_Maps)
    {
        if (x)
            x->update(uiDiff);
    }
}

void MapMgr::unloadMap(MapPtr pMap)
{
    if (pMap)
        unloadMap(pMap->getGUID());
}

void MapMgr::unloadMap(uint32 GUID)
{
    setItem(GUID, MapPtr());
    for (MapList::iterator itr = m_Maps.begin(); itr != m_Maps.end(); ++itr)
    {
        if ((*itr)->getGUID() == GUID)
            m_Maps.erase(itr);
    }
}