#ifndef AUTO_TILE_CACHE_H
#define AUTO_TILE_CACHE_H

#include "AutoTile.h"

class AutoTileCache : public Container<AutoTile>
{
private:
    AutoTile* _createAutoTile(uint32 uiID);

public:
    AutoTileCache(const TileCache &tileCache);

    const AutoTile* getItem(uint32 uiID) const;

private:
    const TileCache &m_TileCache;
};
#endif
