#include "ScriptAreaMappingRevert.h"
#include "MapEditor.h"

using namespace MAPPING_MODE::SCRIPT_AREA::REVERT;

/*#####
# Base
#####*/
Base::Base(MapEditor& editor) : m_Editor(editor), MAP::REVERT::Interface()
{}

/*#####
# Add
#####*/
Add::Add(MAP::GUID guid, MapEditor& editor) : Base(editor), m_GUID(guid)
{}

void Add::revert()
{
    m_Editor.removeScriptArea(m_GUID);
}

/*#####
# Modify
#####*/
Modify::Modify(MAP::SCRIPT_AREA::ScriptArea* pArea, MapEditor& editor) : Base(editor), m_pArea(pArea)
{}

void Modify::revert()
{
}

/*#####
# Remove
#####*/
Remove::Remove(MAP::SCRIPT_AREA::ScriptArea* pArea, MapEditor& editor) : Base(editor), m_pArea(pArea)
{}

void Remove::revert()
{
    m_Editor.addScriptArea(m_pArea.release());
}