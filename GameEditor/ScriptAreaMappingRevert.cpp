#include "ScriptAreaMappingRevert.h"
#include "MapEditor.h"
#include "ScriptAreaItem.h"

using namespace MAPPING_MODE::SCRIPT_AREA::REVERT;

/*#####
# Base
#####*/
Base::Base(MapEditor& editor)
    : m_Editor(editor), MAP::REVERT::Interface()
{}

/*#####
# Add
#####*/
Add::Add(MAP::GUID guid, MapEditor& editor)
    : Base(editor), m_GUID(guid)
{}

void Add::revert()
{
    m_Editor.removeScriptArea(m_GUID);
}

/*#####
# Move
#####*/
Move::Move(MAP::SCRIPT_AREA::ScriptArea* pScriptArea, uint32 index, const GEOMETRY::Point<int32>& pos, MapEditor& editor)
    : Base(editor), m_Index(index), m_Pos(pos), m_pScriptArea(pScriptArea)
{}

void Move::revert()
{
    if (isEmpty())
        return;
    if (auto pScript = m_Editor.getScriptArea(m_pScriptArea->getGUID()))
        pScript->setPoint(m_Index, m_Pos);
}

/*#####
# Remove
#####*/
Remove::Remove(MAP::SCRIPT_AREA::ScriptArea* pArea, MapEditor& editor)
    : Base(editor), m_pArea(pArea)
{}

void Remove::revert()
{
    m_Editor.addScriptArea(m_pArea.release());
}