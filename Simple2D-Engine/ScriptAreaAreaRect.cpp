#include "Map/ScriptArea/AreaRect.h"

using namespace MAP::SCRIPT_AREA::AREA;

AreaRect::AreaRect(const GEOMETRY::Rectangle<int32>& rect) : AreaBase(rect)
{
    int x = 0;
}

void AreaRect::setPosition(const GEOMETRY::Point<int32>& point)
{
    if (m_Area.getPosition() == point)
        return;

    m_Area.setPosition(point);
    setupTriangles();
}

void AreaRect::setSize(const GEOMETRY::Size<int32>& point)
{
    if (m_Area.getSize() == point)
        return;

    m_Area.setSize(point);
    setupTriangles();
}

GEOMETRY::Rectangle<int32> AreaRect::getBoundingRect() const
{
    return m_Area;
}

Data AreaRect::getData() const
{
    return Data(m_Area);
}
