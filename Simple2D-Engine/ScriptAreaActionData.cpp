#include "Map/ScriptArea/ActionData.h"

using namespace MAP::SCRIPT_AREA::ACTION;

Data::Data(uint32 mapID, const GEOMETRY::Point<int32>& pos)
    : m_MapID(mapID), m_Position(pos), m_Type(Type::teleport)
{}

Data::Data(const QString& scriptName)
    : m_ScriptName(scriptName), m_Type(Type::custom_script)
{}

Type Data::getType() const
{
    return m_Type;
}

QString Data::getScriptName() const
{
    return m_ScriptName;
}

uint32 Data::getMapID() const
{
    return m_MapID;
}

const GEOMETRY::Point<int32>& Data::getPosition() const
{
    return m_Position;
}
