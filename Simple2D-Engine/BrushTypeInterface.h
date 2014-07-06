#ifndef BRUSH_TYPE_INTERFACE_H
#define BRUSH_TYPE_INTERFACE_H

#include "MapTile.h"
#include <memory>

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
                virtual void setStartPosition(const UInt32Point& pos) = 0;
                virtual const UInt32Point& getStartPosition() const = 0;
                virtual uint32 getID() const = 0;

                virtual void setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert) = 0;
                virtual void setBorderTiles(const UInt32PointVec& tiles, REVERT::BrushRevert& revert) = 0;
            };
            typedef std::unique_ptr<Interface> TypePtr;
        }
    }
}
#endif
