#ifndef TILE_CACHE_H
#define TILE_CACHE_H

#include "QtGlobal.h"
#include "DatabaseMgr.h"

class TileCache : public SpaceContainer<QPixmap>
{
private:
    bool _createPixmap(uint32 uiID, ConstQPixmapPtr &result);

public:
    TileCache();

    void setTileDB(DATABASE::ConstTileDatabasePtr pTileDB);
    bool getItem(uint32 uiID, ConstQPixmapPtr &result) const;

private:
    DATABASE::ConstTileDatabasePtr m_pTileDB;
};
typedef std::shared_ptr<TileCache> TileCachePtr;
typedef std::shared_ptr<const TileCache> ConstTileCachePtr;
#endif