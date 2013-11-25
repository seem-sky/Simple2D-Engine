#include "TileCache.h"
#ifdef GAME_EDITOR
#include "Config.h"
#endif

using namespace DATABASE;

TileCache::TileCache(const DatabaseMgr &DBMgr) : m_DBMgr(DBMgr)
{}

const QPixmap* TileCache::getItem(uint32 uiID) const
{
    if (auto pPixmap = Container::getItem(uiID))
        return pPixmap;
    else if (auto pPixmap = const_cast<TileCache&>(*this)._createPixmap(uiID))
        return pPixmap;
    return nullptr;
}

QPixmap* TileCache::_createPixmap(uint32 uiID)
{
    if (!m_DBMgr.getTileDatabase())
        return false;

    if (auto pTile = m_DBMgr.getTileDatabase()->getOriginalPrototype(uiID))
    {
        QPixmap *pNewPixmap(nullptr);
#ifdef GAME_EDITOR
        if (createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pTile, pNewPixmap))
#else
        if (createPixmapFromTexturePrototype("projects/untitled/", pTile, *pNewPixmap))
#endif
        {
            setItem(uiID, pNewPixmap);
            return pNewPixmap;
        }
    }
    return nullptr;
}