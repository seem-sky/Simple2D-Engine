#ifndef SCRIPT_AREA_AREA_INTERFACE_H
#define SCRIPT_AREA_AREA_INTERFACE_H

#include <geometry/Rectangle.h>
#include <vector>
#include <Typedefs.h>

namespace MAP
{
    namespace SCRIPT_AREA
    {
        namespace AREA
        {
            enum class Type
            {
                RECT,
                POLYGON
            };

            class Interface
            {
            public:
                virtual bool isValid() const = 0;
                virtual bool contains() const = 0;
                virtual bool intersects() const = 0;

                virtual std::vector<GEOMETRY::Point<int32>> getPoints() const = 0;
                virtual GEOMETRY::Rectangle<int32> getBoundingRect() const = 0;

                virtual Type getType() const = 0;
            };
        }
    }
}
#endif
