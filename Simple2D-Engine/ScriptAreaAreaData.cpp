#include "Map/ScriptArea/AreaData.h"

using namespace MAP::SCRIPT_AREA::AREA;

Data::Data(Type type, const PointVec<int32>& points)
    : m_Type(type), m_Points(points)
{}

Type Data::getType() const
{
    return m_Type;
}

const PointVec<int32>& Data::getPoints() const
{
    return m_Points;
}

// IO
QDataStream& operator<<(QDataStream& stream, const MAP::SCRIPT_AREA::AREA::Data& data)
{
    stream << static_cast<uint8>(data.getType());
    stream << static_cast<uint32>(data.getPoints().size());
    for (auto& p : data.getPoints())
        stream << p.getX() << p.getY();
    return stream;
}

QDataStream& operator>>(QDataStream& stream, MAP::SCRIPT_AREA::AREA::Data& data)
{
    uint8 type = 0;
    stream >> type;

    PointVec<int32> points;
    uint32 size = 0;
    stream >> size;
    points.resize(size);
    for (uint32 i = 0; i < size; ++i)
    {
        GEOMETRY::Point<int32> p;
        stream >> points.at(i).getX();
        stream >> points.at(i).getY();
    }

    data = MAP::SCRIPT_AREA::AREA::Data(static_cast<MAP::SCRIPT_AREA::AREA::Type>(type), points);
    return stream;
}

