#ifndef BRUSH_TYPE_TILE_H
#define BRUSH_TYPE_TILE_H

#include "BrushTypeInterface.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            class Tile : public Interface
            {
            protected:
                void checkAutoTile(MapTileInfo& info) const;

            public:
                Tile(uint32 tileID, Layer& layer, const UInt32Point& pos);

                void setTiles(const MapTileInfoVec& tiles);
                void setBorderTiles(const MapTileInfoVec& tiles);
            };
        }
    }
}
#endif
