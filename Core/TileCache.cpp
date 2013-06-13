#include "TileCache.h"

using namespace DATABASE;

TileCache::TileCache() : Container()
{}

bool TileCache::getItem(uint32 uiID, ConstQPixmapPtr &result) const
{
    if (Container::getItem(uiID, result) || const_cast<TileCache&>(*this)._createPixmap(uiID, result))
        return true;
    return false;
}

void TileCache::setTileDB(DATABASE::ConstTileDatabasePtr pTileDB)
{
    m_pTileDB = pTileDB;
    clear();
    if (m_pTileDB)
        resize(m_pTileDB->getSize(), false);
}

bool TileCache::_createPixmap(uint32 uiID, ConstQPixmapPtr &result)
{
    ConstTilePrototypePtr proto;
    QPixmap newPixmap;
    if (m_pTileDB && m_pTileDB->getItem(uiID, proto) && createPixmapFromTexturePrototype(proto, newPixmap))
    {
        QPixmapPtr pPixmap = QPixmapPtr(new QPixmap(newPixmap));
        setItem(uiID, pPixmap);
        result = pPixmap;
        return true;
    }
    return false;
}