#include "MapEditorModuleWorldObjects.h"

MapEditorModuleWorldObjects::MapEditorModuleWorldObjects(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : DatabaseModuleDragList(pParent), m_DBMgr(DBMgr)
{
    setMaximumWidth(200);
}

