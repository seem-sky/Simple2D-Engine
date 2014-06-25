#ifndef BRUSH_AREA_PEN_H
#define BRUSH_AREA_PEN_H

#include "BrushAreaInterface.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            class Single : public Interface
            {
            protected:
                void storeBorder(const UInt32Point& pos, MapTileInfoVec& borderTiles, Bitset2D& bitset);

            public:
                Single(Layer& layer, const UInt32Point& pos);

                void start(MapTileInfoVec& tiles, MapTileInfoVec& borderTiles);

            private:
                MapTile m_OldTile;
            };
        }
    }
}
#endif
