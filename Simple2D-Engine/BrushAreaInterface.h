#ifndef BRUSH_AREA_INTERFACE_H
#define BRUSH_AREA_INTERFACE_H

#include "MapTile.h"
#include <memory>
#include <Typedefs.h>

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            class Interface
            {
            public:
                virtual const GEOMETRY::Point<uint32>& getStartPosition() const = 0;
                virtual void setStartPosition(const GEOMETRY::Point<uint32>& pos) = 0;

                virtual void start(MapTileInfoVec& tiles, PointVec<uint32>& borderTiles) = 0;

                virtual void setBrushSize(const GEOMETRY::Point<uint32>& size) = 0;
                virtual GEOMETRY::Point<uint32> getBrushSize() const = 0;
            };
            typedef std::unique_ptr<Interface> AreaPtr;
        }
    }
}
#endif
