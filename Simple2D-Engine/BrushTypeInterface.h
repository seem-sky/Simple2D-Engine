#ifndef BRUSH_TYPE_INTERFACE_H
#define BRUSH_TYPE_INTERFACE_H

#include "MapTile.h"

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
                virtual const UInt32Point& getStartPosition() const = 0;
                virtual uint32 getTileID() const = 0;

                virtual void setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert) = 0;
                virtual void setBorderTiles(const UInt32PointVec& tiles, REVERT::BrushRevert& revert) = 0;
            };
            typedef std::unique_ptr<Interface> TypePtr;
        }
    }
}
#endif
