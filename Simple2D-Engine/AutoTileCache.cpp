#include "AutoTileCache.h"
#include "TileCache.h"

AutoTileCache::AutoTileCache() : Container() {}

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
    if (!m_pDBMgr || !m_pDBMgr->getAutoTileDatabase())
        return nullptr;
    if (auto pAutoTile = m_pDBMgr->getAutoTileDatabase()->getOriginalPrototype(uiID))
    {
        auto pNewAutoTile(new AutoTile(*m_pDBMgr, uiID));
        setItem(uiID, pNewAutoTile);
        return pNewAutoTile;
    }
    return nullptr;
}
