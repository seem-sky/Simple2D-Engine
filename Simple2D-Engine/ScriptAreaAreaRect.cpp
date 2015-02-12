#include "Map/ScriptArea/AreaRect.h"

using namespace MAP::SCRIPT_AREA::AREA;

AreaRect::AreaRect(const GEOMETRY::Rectangle<int32>& rect) : AreaBase(rect)
{
    int x = 0;
}

void AreaRect::setTopLeft(const GEOMETRY::Point<int32>& point)
{
    if (m_Area.getTopLeft() == point)
        return;

    m_Area.setTopLeft(point);
    setupTriangles();
}

void AreaRect::setBottomRight(const GEOMETRY::Point<int32>& point)
{
    if (m_Area.getBottomRight() == point)
        return;

    m_Area.setBottomRight(point);
    setupTriangles();
}

GEOMETRY::Rectangle<int32> AreaRect::getBoundingRect() const
{
    return m_Area;
}
