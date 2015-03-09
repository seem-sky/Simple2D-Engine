#ifndef BRUSH_TYPE_AUTO_TILE_H
#define BRUSH_TYPE_AUTO_TILE_H

#include "BrushType.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            class AutoTile : public Type
            {
            public:
                AutoTile(uint32 tileID, const database::Manager& DBMgr, Layer& layer, const GEOMETRY::Point<uint32>& pos);

                void setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert);
            };
        }
    }
}
#endif
