#ifndef CACHE_TILES_H
#define CACHE_TILES_H

#include "TextureAtlas.h"
#include "TileCacheInfo.h"
#include "Simple2D-Global.h"

namespace DATABASE
{
    class DatabaseMgr;
}

namespace CACHE
{
    class Tiles
    {
    public:
        Tiles(const DATABASE::DatabaseMgr& DBMgr);

        TileCacheInfo get(uint32 ID) const;
        void clear();

    private:
        const DATABASE::DatabaseMgr& m_DBMgr;
        TextureAtlas<MAP::TILE_SIZE, MAP::TILE_SIZE> m_Atlas;
    };
}
#endif
