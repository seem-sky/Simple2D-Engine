#ifndef AUTO_TILE_CACHE_H
#define AUTO_TILE_CACHE_H

#include "AutoTile.h"

class AutoTileCache : public Container<AutoTile>
{
private:
    bool _createAutoTile(uint32 uiID, ConstAutoTilePtr &result);

public:
    AutoTileCache(ConstTileCachePtr pTileCache);

    void setAutoTileDB(DATABASE::ConstAutoTileDatabasePtr pAutoTileDB);
    bool getItem(uint32 uiID, ConstAutoTilePtr &result) const;

private:
    ConstTileCachePtr m_pTileCache;
    DATABASE::ConstAutoTileDatabasePtr m_pAutoTileDB;
};
typedef std::shared_ptr<AutoTileCache> AutoTileCachePtr;
typedef std::shared_ptr<const AutoTileCache> ConstAutoTileCachePtr;
#endif