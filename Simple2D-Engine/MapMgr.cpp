#include "MapMgr.h"

using namespace MAP;

MapMgr::MapMgr() : Manager()
{}

void MapMgr::setDatabaseMgr(DATABASE::ConstDatabaseMgrPtr pDBMgr)
{
    m_pDBMgr = pDBMgr;
}

MapPtr MapMgr::loadMap(uint32 uiID) const
{
    MapPtr pMap;
    DATABASE::MAP_STRUCTURE::MapPrototypePtr pMapProto;
    DATABASE::MapDatabasePtr pMapDB = std::const_pointer_cast<DATABASE::MAP_STRUCTURE::MapDatabase>(m_pDBMgr->getMapDatabase());
    if (pMapDB && pMapDB->loadMapFile(uiID, "projects/untitled/") && pMapDB->getItem(uiID, pMapProto))
        pMap = MapPtr(new Map(pMapProto->getID(), pMapProto->getName().toStdString(), pMapProto->m_Layer));
    return std::move(pMap);
}