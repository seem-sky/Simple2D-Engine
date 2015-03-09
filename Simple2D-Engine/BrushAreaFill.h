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
                void _do(const MapTileInfo& info, MapTileInfoVec& tiles, PointVec<uint32>& borderTiles);
                void _checkTile(const GEOMETRY::Point<uint32>& pos, sl::Bitset2D& bitset, sl::Bitset2D& borderBitset,
                    PointVec<uint32>& openPositions, MapTileInfoVec& tileInfos, PointVec<uint32>& borderTiles);

            public:
                Fill(Layer& layer, const GEOMETRY::Point<uint32>& pos);
            };
        }
    }
}
#endif
