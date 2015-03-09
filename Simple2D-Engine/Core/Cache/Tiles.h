#ifndef CACHE_TILES_H
#define CACHE_TILES_H

#include "TileBase.h"

namespace CACHE
{
    class Tiles : public TileBase<MAP::TILE_SIZE, MAP::TILE_SIZE, 1024, 1024>
    {
    public:
        Tiles(const database::Manager& DBMgr);

        TileCacheInfo get(uint32 ID) const;
    };
}
#endif
