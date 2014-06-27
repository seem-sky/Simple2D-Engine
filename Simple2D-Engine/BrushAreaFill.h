#ifndef BRUSH_AREA_FILL_H
#define BRUSH_AREA_FILL_H

#include "BrushArea.h"
#include <Bitset2D.h>

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            class Fill : public Area
            {
            private:
                void _do(const MapTileInfo& info, MapTileInfoVec& tiles, UInt32PointVec& borderTiles);
                void _checkTile(const UInt32Point& pos, Bitset2D& bitset, Bitset2D& borderBitset,
                    UInt32PointVec& openPositions, MapTileInfoVec& tileInfos, UInt32PointVec& borderTiles);

            public:
                Fill(Layer& layer, const UInt32Point& pos);
            };
        }
    }
}
#endif
