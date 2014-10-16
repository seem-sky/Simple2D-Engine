#include "ObjectMappingRevert.h"
#include "MapEditor.h"

using namespace MAPPING_MODE::REVERT;

/*#####
# ObjectBase
#####*/
ObjectBase::ObjectBase(MapEditor& editor) : m_Editor(editor), MAP::REVERT::Interface()
{}

/*#####
# ObjectAdd
#####*/
ObjectAdd::ObjectAdd(MAP::MAP_DATA::GUID guid, MapEditor& editor) : ObjectBase(editor), m_GUID(guid)
{}

void ObjectAdd::revert()
{
    m_Editor.removeWorldObject(m_GUID);
}

/*#####
# ObjectModify
#####*/
ObjectModify::ObjectModify(const MAP::MAP_DATA::WorldObjectInfo& info, MapEditor& editor) : ObjectBase(editor), m_Info(info)
{}

void ObjectModify::revert()
{
    m_Editor.setWorldObject(m_Info);
}

/*#####
# ObjectRemove
#####*/
ObjectRemove::ObjectRemove(const MAP::MAP_DATA::WorldObjectInfo& info, MapEditor& editor) : ObjectBase(editor), m_Info(info)
{}

void ObjectRemove::revert()
{
    m_Editor.addWorldObject(m_Info);
}