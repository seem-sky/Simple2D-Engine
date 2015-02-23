#include "Map/ScriptArea/Actions.h"

using namespace MAP::SCRIPT_AREA::ACTION;

/*#####
# Teleport
#####*/
Teleport::Teleport(uint32 mapID, const GEOMETRY::Point<int32>& pos)
    : m_MapID(mapID), m_Position(pos)
{}

Type Teleport::getType() const
{
    return Type::teleport;
}

Data Teleport::getData() const
{
    return Data(m_MapID, m_Position);
}

void Teleport::setMapID(uint32 ID)
{
    m_MapID = ID;
}

uint32 Teleport::getMapID() const
{
    return m_MapID;
}

void Teleport::setPosition(const GEOMETRY::Point<int32>& pos)
{
    m_Position = pos;
}

GEOMETRY::Point<int32> Teleport::getPosition() const
{
    return m_Position;
}

/*#####
# CustomScript
#####*/
CustomScript::CustomScript(const QString& scriptName)
    : m_ScriptName(scriptName)
{}

Type CustomScript::getType() const
{
    return Type::custom_script;
}

Data CustomScript::getData() const
{
    return Data(m_ScriptName);
}

void CustomScript::setScriptName(const QString& script)
{
    m_ScriptName = script;
}

const QString& CustomScript::getScriptName() const
{
    return m_ScriptName;
}
