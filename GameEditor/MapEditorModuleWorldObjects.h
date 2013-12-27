#ifndef MAP_EDITOR_MODULE_WORLD_OBJECTS_H
#define MAP_EDITOR_MODULE_WORLD_OBJECTS_H

#include "DatabaseModuleDragDrop.h"
#include "DatabaseMgr.h"

class MapEditorModuleWorldObjects : public DatabaseModuleDragList
{
public:
    MapEditorModuleWorldObjects(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);

private:
    const DATABASE::DatabaseMgr& m_DBMgr;
};

#endif
