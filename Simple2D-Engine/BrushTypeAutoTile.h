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
                AutoTile(uint32 tileID, const DATABASE::DatabaseMgr& DBMgr, Layer& layer, const UInt32Point& pos);

                void setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert);
            };
        }
    }
}
#endif
