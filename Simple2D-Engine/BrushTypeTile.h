#ifndef BRUSH_TYPE_TILE_H
#define BRUSH_TYPE_TILE_H

#include "BrushType.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            class Tile : public Type
            {
            public:
                Tile(uint32 tileID, const DATABASE::DatabaseMgr& DBMgr, Layer& layer, const UInt32Point& pos);

                void setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert);
            };
        }
    }
}
#endif
