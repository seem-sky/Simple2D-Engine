#ifndef BRUSH_AREA_SINGLE_H
#define BRUSH_AREA_SINGLE_H

#include "BrushArea.h"
#include <Bitset2D.h>

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            class Single : public Area
            {
            private:
                void _do(const MapTileInfo& info, MapTileInfoVec& tiles, UInt32PointVec& borderTiles);
                void _storeBorder(const UInt32Point& pos, UInt32PointVec& borderTiles);

            public:
                Single(Layer& layer, const UInt32Point& pos);
            };
        }
    }
}
#endif
