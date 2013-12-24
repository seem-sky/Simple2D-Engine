#include "TileCache.h"
#include "Config.h"

using namespace DATABASE;

TileCache::TileCache() : m_pDBMgr(nullptr)
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
    if (!m_pDBMgr || !m_pDBMgr->getTileDatabase())
        return nullptr;

    if (auto pTile = m_pDBMgr->getTileDatabase()->getOriginalPrototype(uiID))
    {
        std::unique_ptr<QPixmap> pixmapPtr(new QPixmap);
        if (createPixmap(Config::get()->getProjectDirectory(), pTile->getPathName(), pTile->getTransparencyColor(), *pixmapPtr))
        {
            auto pPixmap = pixmapPtr.release();
            setItem(uiID, pPixmap);
            return pPixmap;
        }
    }
    return nullptr;
}