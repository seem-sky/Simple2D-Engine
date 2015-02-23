#ifndef MAP_SCRIPT_AREA_AREA_RECT_H
#define MAP_SCRIPT_AREA_AREA_RECT_H

#include "AreaBase.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        namespace AREA
        {
            class AreaRect : public AreaBase<GEOMETRY::Rectangle<int32>>
            {
            public:
                AreaRect(const GEOMETRY::Rectangle<int32>& rect);

                void setPoint(uint32 i, const GEOMETRY::Point<int32>& p);
                void setPosition(const GEOMETRY::Point<int32>& point);
                void setSize(const GEOMETRY::Size<int32>& point);

                GEOMETRY::Rectangle<int32> getBoundingRect() const;

                Data getData() const;
                Type getType() const { return Type::rect; }
                void moveBy(int32 x, int32 y);
                void moveBy(const GEOMETRY::Point<int32>& pos);
            };
        }
    }
}
#endif
