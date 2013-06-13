#ifndef TILE_CACHE_H
#define TILE_CACHE_H

#include "QtGlobal.h"
#include "DatabaseMgr.h"

class TileCache : public Container<QPixmap>
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
typedef boost::shared_ptr<TileCache> TileCachePtr;
typedef boost::shared_ptr<const TileCache> ConstTileCachePtr;
#endif