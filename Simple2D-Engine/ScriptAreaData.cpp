#include "Map/ScriptArea/Data.h"

using namespace MAP::SCRIPT_AREA;

Data::Data(MAP::GUID guid, const AREA::Data& areaData, const ACTION::Data& actionData)
    : m_ActionData(actionData), m_AreaData(areaData), m_Guid(guid)
{}

const AREA::Data& Data::getAreaData() const
{
    return m_AreaData;
}

const ACTION::Data& Data::getActionData() const
{
    return m_ActionData;
}

MAP::GUID Data::getGUID() const
{
    return m_Guid;
}

bool Data::isValid() const
{
    return m_Guid != 0;
}

// IO
QDataStream& operator<<(QDataStream& stream, const MAP::SCRIPT_AREA::Data& data)
{
    stream << data.getGUID();
    stream << data.getAreaData();
    return stream;
}

QDataStream& operator>>(QDataStream& stream, MAP::SCRIPT_AREA::Data& data)
{
    MAP::GUID guid;
    stream >> guid;

    MAP::SCRIPT_AREA::AREA::Data areaData;
    stream >> areaData;
    data = MAP::SCRIPT_AREA::Data(guid, areaData, MAP::SCRIPT_AREA::ACTION::Data());
    return stream;
}
