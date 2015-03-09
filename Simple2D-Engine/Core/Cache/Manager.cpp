#include "Manager.h"
#include <log/Log.h>

using namespace CACHE;

Manager::Manager(const database::Manager& DBMgr)
    : m_TileCache(DBMgr), m_AutoTileCache(m_TileCache, DBMgr)
{}

void Manager::clear()
{
    m_TileCache.clear();
    m_AutoTileCache.clear();

    STANDARD_MESSAGE(std::string("CacheManager: Caches cleared."));
}

Tiles& Manager::getTileCache()
{
    return m_TileCache;
}

AutoTiles& Manager::getAutoTileCache()
{
    return m_AutoTileCache;
}
