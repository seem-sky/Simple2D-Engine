#include "Map/ScriptArea/ScriptArea.h"
#include "Map/ScriptArea/AreaRect.h"
#include <math.h>

using namespace MAP::SCRIPT_AREA;

ScriptArea::ScriptArea(MAP::GUID guid) : m_GUID(guid)
{
    if (!m_GUID)
        throw std::runtime_error("GUID can not be 0.");
}

AREA::Interface* ScriptArea::getArea()
{
    return m_pArea.get();
}
const AREA::Interface* ScriptArea::getArea() const
{
    return m_pArea.get();
}

void ScriptArea::setArea(const AREA::Data& data)
{
    m_pArea = std::unique_ptr<AREA::Interface>(AREA::create(data));
}

const ACTION::Interface* ScriptArea::getAction() const
{
    return m_pAction.get();
}

ACTION::Interface* ScriptArea::getAction()
{
    return m_pAction.get();
}

void ScriptArea::setAction(const ACTION::Data& data)
{
    m_pAction = std::unique_ptr<ACTION::Interface>(ACTION::create(data));
}

Data ScriptArea::getData() const
{
    return Data(getGUID(), m_pArea->getData(), ACTION::Data());
}

// free functions
AREA::Interface* AREA::create(const AREA::Data& data)
{
    std::unique_ptr<AREA::Interface> pArea;
    switch (data.getType())
    {
    case AREA::Type::rect:
        GEOMETRY::Rectangle<int32> rect;
        for (uint32 i = 0; i < rect.pointCount(); ++i)
        {
            if (data.getPoints().size() <= i)
                break;
            rect.setPoint(i, data.getPoints().at(i));
        }
        
        // resize to at least minimum size
        rect.setSize(MINIMUM_SIZE, MINIMUM_SIZE);
        pArea = std::unique_ptr<AREA::Interface>(new AREA::AreaRect(rect));
        break;
    }

    return pArea.release();
}

ACTION::Interface* ACTION::create(const ACTION::Data& data)
{
    std::unique_ptr<ACTION::Interface> pAction;
    switch (data.getType())
    {
    case ACTION::Type::teleport:
        break;
    case ACTION::Type::custom_script:
        break;
    }
    return pAction.release();
}
