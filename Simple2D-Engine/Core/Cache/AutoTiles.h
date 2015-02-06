#ifndef CACHE_AUTO_TILES_H
#define CACHE_AUTO_TILES_H

#include "TextureAtlas.h"
#include <memory>
#include "TileCacheInfo.h"
#include "Simple2D-Global.h"

namespace DATABASE
{
    class DatabaseMgr;
}

namespace CACHE
{
    class Tiles;

    const uint16 ATLAS_WIDTH = 4, ATLAS_HEIGHT = 10;
    class AutoTiles
    {
    private:
        void _setupAutoTile(uint32 ID);

    public:
        AutoTiles(Tiles& tileCache, const DATABASE::DatabaseMgr& DBMgr);

        TileCacheInfo get(uint32 ID, DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_INDEX index) const;
        void clear();

    private:
        const DATABASE::DatabaseMgr& m_DBMgr;
        TextureAtlas<ATLAS_WIDTH * MAP::TILE_SIZE, ATLAS_HEIGHT * MAP::TILE_SIZE> m_Atlas;
        Tiles& m_TileCache;
    };
}
#endif
