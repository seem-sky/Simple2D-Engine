#include "ObjectMappingRevert.h"
#include "MapEditor.h"

using namespace MAPPING_MODE::OBJECT::REVERT;

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
    m_Editor.removeWorldObject(m_GUID);
}

/*#####
# Modify
#####*/
Modify::Modify(const MAP::MAP_DATA::WorldObjectInfo& info, MapEditor& editor) : Base(editor), m_Info(info)
{}

void Modify::revert()
{
    m_Editor.setWorldObject(m_Info);
}

/*#####
# Remove
#####*/
Remove::Remove(const MAP::MAP_DATA::WorldObjectInfo& info, MapEditor& editor) : Base(editor), m_Info(info)
{}

void Remove::revert()
{
    m_Editor.addWorldObject(m_Info);
}