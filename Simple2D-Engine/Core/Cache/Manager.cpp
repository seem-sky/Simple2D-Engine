#include "Manager.h"

using namespace CACHE;

Manager::Manager(const DATABASE::DatabaseMgr& DBMgr)
    : m_TileCache(DBMgr), m_AutoTileCache(m_TileCache, DBMgr)
{}

void Manager::clear()
{
    m_TileCache.clear();
    m_AutoTileCache.clear();
}

Tiles& Manager::getTileCache()
{
    return m_TileCache;
}

AutoTiles& Manager::getAutoTileCache()
{
    return m_AutoTileCache;
}
