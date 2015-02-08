#ifndef CACHE_AUTO_TILES_H
#define CACHE_AUTO_TILES_H

#include "TileBase.h"

namespace CACHE
{
    class Tiles;

    const uint16 ATLAS_WIDTH = 4, ATLAS_HEIGHT = 10;
    class AutoTiles : public TileBase<ATLAS_WIDTH * MAP::TILE_SIZE, ATLAS_HEIGHT * MAP::TILE_SIZE, 1024, 1024>
    {
    private:
        TileCacheInfo _setupAutoTile(uint32 ID);

    public:
        AutoTiles(Tiles& tileCache, const DATABASE::DatabaseMgr& DBMgr);

        TileCacheInfo get(uint32 ID, DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_INDEX index) const;

    private:
        Tiles& m_TileCache;
    };
}
#endif
