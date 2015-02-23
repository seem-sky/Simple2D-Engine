#include "Map/ScriptArea/AreaRect.h"
#include "Simple2D-Global.h"

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

    GEOMETRY::Size<int32> size(std::max(MAP::SCRIPT_AREA::MINIMUM_SIZE, point.getWidth()), std::max(MAP::SCRIPT_AREA::MINIMUM_SIZE, point.getHeight()));
    m_Area.setSize(size);
    setupTriangles();
}

void AreaRect::moveBy(int32 x, int32 y)
{
    m_Area.moveBy(x, y);
}

void AreaRect::moveBy(const GEOMETRY::Point<int32>& pos)
{
    m_Area.moveBy(pos);
}

GEOMETRY::Rectangle<int32> AreaRect::getBoundingRect() const
{
    return m_Area;
}

Data AreaRect::getData() const
{
    return Data(m_Area);
}

void AreaRect::setPoint(uint32 i, const GEOMETRY::Point<int32>& p)
{
    auto rect = m_Area;
    // get sure that the size is not smaller than MAP::SCRIPT_AREA::MINIMUM_SIZE
    switch (i)
    {
    case 0:     // top left
    {
        auto diff = p - rect.getPosition();
        if (diff.getX() == 0 && diff.getY() == 0)
            return;

        rect.moveBy(diff);
        rect.setWidth(rect.getWidth() - diff.getX());
        rect.setHeight(rect.getHeight() - diff.getY());
        // x
        if (rect.getWidth() < MAP::SCRIPT_AREA::MINIMUM_SIZE)
        {
            rect.setX(rect.getX() - MAP::SCRIPT_AREA::MINIMUM_SIZE + rect.getWidth());
            rect.setWidth(MAP::SCRIPT_AREA::MINIMUM_SIZE);
        }

        // y
        if (rect.getHeight() < MAP::SCRIPT_AREA::MINIMUM_SIZE)
        {
            rect.setY(rect.getY() - MAP::SCRIPT_AREA::MINIMUM_SIZE + rect.getHeight());
            rect.setHeight(MAP::SCRIPT_AREA::MINIMUM_SIZE);
        }
        break;
    }
    case 1:     // top right
    {
        GEOMETRY::Point<int32> diff(p.getX() - (rect.getX() + rect.getWidth()), p.getY() - rect.getY());
        if (diff.getX() == 0 && diff.getY() == 0)
            return;

        rect.moveBy(0, diff.getY());
        rect.setWidth(rect.getWidth() + diff.getX());
        rect.setHeight(rect.getHeight() - diff.getY());
        // x
        if (rect.getWidth() < MAP::SCRIPT_AREA::MINIMUM_SIZE)
            rect.setWidth(MAP::SCRIPT_AREA::MINIMUM_SIZE);

        // y
        if (rect.getHeight() < MAP::SCRIPT_AREA::MINIMUM_SIZE)
        {
            rect.setY(rect.getY() - MAP::SCRIPT_AREA::MINIMUM_SIZE + rect.getHeight());
            rect.setHeight(MAP::SCRIPT_AREA::MINIMUM_SIZE);
        }
        break;
    }

    case 2:     // bottom right
    {
        GEOMETRY::Point<int32> diff(p.getX() - (rect.getX() + rect.getWidth()), p.getY() - (rect.getY() + rect.getHeight()));
        if (diff.getX() == 0 && diff.getY() == 0)
            return;

        rect.setWidth(rect.getWidth() + diff.getX());
        rect.setHeight(rect.getHeight() + diff.getY());

        // x
        if (rect.getWidth() < MAP::SCRIPT_AREA::MINIMUM_SIZE)
            rect.setWidth(MAP::SCRIPT_AREA::MINIMUM_SIZE);

        // x
        if (rect.getHeight() < MAP::SCRIPT_AREA::MINIMUM_SIZE)
            rect.setHeight(MAP::SCRIPT_AREA::MINIMUM_SIZE);
        break;
    }

    case 3:     // bottom left
    {
        GEOMETRY::Point<int32> diff(p.getX() - rect.getX(), p.getY() - (rect.getY() + rect.getHeight()));
        if (diff.getX() == 0 && diff.getY() == 0)
            return;

        rect.moveBy(diff.getX(), 0);
        rect.setWidth(rect.getWidth() - diff.getX());
        rect.setHeight(rect.getHeight() + diff.getY());

        // x
        if (rect.getWidth() < MAP::SCRIPT_AREA::MINIMUM_SIZE)
        {
            rect.setX(rect.getX() - MAP::SCRIPT_AREA::MINIMUM_SIZE + rect.getWidth());
            rect.setWidth(MAP::SCRIPT_AREA::MINIMUM_SIZE);
        }

        // y
        if (rect.getHeight() < MAP::SCRIPT_AREA::MINIMUM_SIZE)
            rect.setHeight(MAP::SCRIPT_AREA::MINIMUM_SIZE);
        break;
    }
    }
    m_Area = rect;
}
