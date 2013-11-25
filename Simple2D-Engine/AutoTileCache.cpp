#include "AutoTileCache.h"

AutoTileCache::AutoTileCache(const TileCache &tileCache) : Container(), m_TileCache(tileCache) {}

const AutoTile* AutoTileCache::getItem(uint32 uiID) const
{
    if (auto pAutoTile = Container::getItem(uiID))
        return pAutoTile;
    else if (auto pAutoTile = const_cast<AutoTileCache&>(*this)._createAutoTile(uiID))
        return pAutoTile;
    return nullptr;
}

AutoTile* AutoTileCache::_createAutoTile(uint32 uiID)
{
    if (!m_TileCache.getDBMgr().getAutoTileDatabase())
        return nullptr;
    if (auto pAutoTile = m_TileCache.getDBMgr().getAutoTileDatabase()->getOriginalPrototype(uiID))
    {
        auto pNewAutoTile(new AutoTile());
        pNewAutoTile->setAutoTilePrototype(pAutoTile, m_TileCache);
        setItem(uiID, pNewAutoTile);
        return pNewAutoTile;
    }
    return nullptr;
}
