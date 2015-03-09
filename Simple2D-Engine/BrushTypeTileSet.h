#ifndef BRUSH_TYPE_TILE_SET_H
#define BRUSH_TYPE_TILE_SET_H

#include "BrushType.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            class TileSet : public Type
            {
            public:
                TileSet(uint32 tileID, const database::Manager& DBMgr, Layer& layer, const GEOMETRY::Point<uint32>& pos);

                void setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert);
            };
        }
    }
}
#endif
