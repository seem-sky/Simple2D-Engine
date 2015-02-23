#include "Map/ScriptArea/ScriptArea.h"
#include "Map/ScriptArea/AreaRect.h"
#include <math.h>

using namespace MAP::SCRIPT_AREA;

ScriptArea::ScriptArea(MAP::GUID guid)
    : m_GUID(guid)
{
    if (!m_GUID)
        throw std::runtime_error("GUID not allowed to be 0.");
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

bool ScriptArea::isEmpty() const
{
    return !m_GUID || !m_pAction || !m_pArea;
}

// free functions
AREA::Interface* AREA::create(const AREA::Data& data)
{
    std::unique_ptr<AREA::Interface> pArea;
    switch (data.getType())
    {
    case AREA::Type::rect:
        // resize to at least minimum size
        pArea = std::unique_ptr<AREA::Interface>(new AREA::AreaRect(
            GEOMETRY::Rectangle<int32>(data.getRectangle().getPosition(), GEOMETRY::Size<int32>(MINIMUM_SIZE, MINIMUM_SIZE))));
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
