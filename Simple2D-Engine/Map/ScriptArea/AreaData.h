#ifndef MAP_SCRIPT_AREA_AREA_DATA_H
#define MAP_SCRIPT_AREA_AREA_DATA_H

#include <Typedefs.h>
#include <QtCore/QDataStream>
#include <geometry/Rectangle.h>

namespace MAP
{
    namespace SCRIPT_AREA
    {
        namespace AREA
        {
            enum class Type
            {
                none,
                rect,
                polygon
            };

            class Data
            {
            public:
                Data() = default;
                Data(const GEOMETRY::Rectangle<int32>& rect);
                Data(const PointVec<int32>& points);

                Type getType() const;
                const PointVec<int32>& getPoints() const;
                const GEOMETRY::Rectangle<int32>& getRectangle() const;

            private:
                Type m_Type = Type::none;

                PointVec<int32> m_Points;
                GEOMETRY::Rectangle<int32> m_Rectangle;
            };
        }
    }
}

// IO
QDataStream& operator<<(QDataStream& stream, const MAP::SCRIPT_AREA::AREA::Data& data);
QDataStream& operator>>(QDataStream& stream, MAP::SCRIPT_AREA::AREA::Data& data);

#endif
