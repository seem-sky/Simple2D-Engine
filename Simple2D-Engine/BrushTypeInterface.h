#ifndef BRUSH_TYPE_INTERFACE_H
#define BRUSH_TYPE_INTERFACE_H

#include "MapTile.h"
#include <memory>
#include <Typedefs.h>

namespace MAP
{
    namespace BRUSH
    {
        namespace REVERT
        {
            class BrushRevert;
        }

        namespace TYPE
        {
            class Interface
            {
            public:
                virtual void setStartPosition(const GEOMETRY::Point<uint32>& pos) = 0;
                virtual const GEOMETRY::Point<uint32>& getStartPosition() const = 0;
                virtual uint32 getID() const = 0;

                virtual void setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert) = 0;
                virtual void setBorderTiles(const PointVec<uint32>& tiles, REVERT::BrushRevert& revert) = 0;
            };
            typedef std::unique_ptr<Interface> TypePtr;
        }
    }
}
#endif
