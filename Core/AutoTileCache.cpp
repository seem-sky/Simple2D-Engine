#include "AutoTileCache.h"

AutoTileCache::AutoTileCache(ConstTileCachePtr pTileCache) : Container(), m_pTileCache(pTileCache) {}

bool AutoTileCache::getItem(uint32 uiID, ConstAutoTilePtr &result) const
{
    if (Container::getItem(uiID, result) || const_cast<AutoTileCache&>(*this)._createAutoTile(uiID, result))
        return true;
    return false;
}

void AutoTileCache::setAutoTileDB(DATABASE::ConstAutoTileDatabasePtr pAutoTileDB)
{
    m_pAutoTileDB = pAutoTileDB;
    clear();
    if (m_pAutoTileDB)
        resize(m_pAutoTileDB->getSize(), false);
}

bool AutoTileCache::_createAutoTile(uint32 uiID, ConstAutoTilePtr &result)
{
    DATABASE::ConstAutoTilePrototypePtr proto;
    if (m_pTileCache && m_pAutoTileDB && m_pAutoTileDB->getItem(uiID, proto))
    {
        AutoTilePtr pNewAutoTile(new AutoTile());
        pNewAutoTile->setAutoTilePrototype(proto, m_pTileCache);
        setItem(uiID, pNewAutoTile);
        result = pNewAutoTile;
        return true;
    }
    return false;
}