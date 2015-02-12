#ifndef MAP_SCRIPT_AREA_AREA_INTERFACE_H
#define MAP_SCRIPT_AREA_AREA_INTERFACE_H

#include <geometry/Rectangle.h>
#include <vector>
#include <Typedefs.h>
#include "AreaData.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        namespace AREA
        {
            class Interface
            {
            public:
                virtual bool isValid() const = 0;
                virtual bool contains() const = 0;
                virtual bool intersects() const = 0;

                virtual GEOMETRY::Point<int32> getPoint(uint32 i) const = 0;
                virtual void setPoint(uint32 i, const GEOMETRY::Point<int32>& p) = 0;
                virtual uint32 pointCount() const = 0;
                virtual std::vector<GEOMETRY::Point<int32>> getPoints() const = 0;
                virtual GEOMETRY::Rectangle<int32> getBoundingRect() const = 0;

                virtual Type getType() const = 0;

                virtual Data getData() const = 0;
            };
        }
    }
}
#endif
