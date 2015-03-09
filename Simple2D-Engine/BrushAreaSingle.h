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
                void _do(const MapTileInfo& info, MapTileInfoVec& tiles, PointVec<uint32>& borderTiles);
                void _storeBorder(const GEOMETRY::Point<uint32>& pos, PointVec<uint32>& borderTiles, sl::Bitset2D& bitset);

            public:
                Single(Layer& layer, const GEOMETRY::Point<uint32>& pos);
            };
        }
    }
}
#endif
