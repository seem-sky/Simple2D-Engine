#include "Map/ScriptArea/AreaData.h"

using namespace MAP::SCRIPT_AREA::AREA;

Data::Data(const GEOMETRY::Rectangle<int32>& rect)
    : m_Type(Type::rect), m_Rectangle(rect)
{}

Data::Data(const PointVec<int32>& points)
    : m_Type(Type::polygon), m_Points(points)
{}

Type Data::getType() const
{
    return m_Type;
}

const PointVec<int32>& Data::getPoints() const
{
    return m_Points;
}

const GEOMETRY::Rectangle<int32>& Data::getRectangle() const
{
    return m_Rectangle;
}

// IO
QDataStream& operator<<(QDataStream& stream, const MAP::SCRIPT_AREA::AREA::Data& data)
{
    stream << static_cast<uint8>(data.getType());
    switch (data.getType())
    {
    case Type::rect:
        stream << data.getRectangle().getX() << data.getRectangle().getY() <<
            data.getRectangle().getWidth() << data.getRectangle().getHeight();
        break;
    case Type::polygon:
        stream << static_cast<uint32>(data.getPoints().size());
        for (auto& p : data.getPoints())
            stream << p.getX() << p.getY();
        break;
    }
    return stream;
}

QDataStream& operator>>(QDataStream& stream, MAP::SCRIPT_AREA::AREA::Data& data)
{
    uint8 type = 0;
    stream >> type;

    switch (static_cast<Type>(type))
    {
    case Type::rect:
    {
        GEOMETRY::Point<int32> point;
        GEOMETRY::Size<int32> size;
        stream >> point.getX() >> point.getY() >> size.getWidth() >> size.getHeight();
        data = MAP::SCRIPT_AREA::AREA::Data(GEOMETRY::Rectangle<int32>(point, size));
        break;
    }

    case Type::polygon:
    {
        PointVec<int32> points;
        uint32 size = 0;
        stream >> size;
        points.resize(size);
        for (uint32 i = 0; i < size; ++i)
        {
            GEOMETRY::Point<int32> p;
            stream >> points.at(i).getX() >> points.at(i).getY();
        }
        data = MAP::SCRIPT_AREA::AREA::Data(points);
        break;
    }
    }
    return stream;
}

