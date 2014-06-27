#ifndef BRUSH_AREA_INTERFACE_H
#define BRUSH_AREA_INTERFACE_H

#include "MapTile.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            class Interface
            {
            public:
                virtual const UInt32Point& getStartPosition() const = 0;
                virtual void setStartPosition(const UInt32Point& pos) = 0;

                virtual void start(MapTileInfoVec& tiles, UInt32PointVec& borderTiles) = 0;

                virtual void setBrushSize(const UInt32Point& size) = 0;
                virtual UInt32Point getBrushSize() const = 0;
            };
            typedef std::unique_ptr<Interface> AreaPtr;
        }
    }
}
#endif
