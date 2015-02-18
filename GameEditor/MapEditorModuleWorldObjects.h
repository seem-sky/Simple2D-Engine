#ifndef MAP_EDITOR_MODULE_WORLD_OBJECTS_H
#define MAP_EDITOR_MODULE_WORLD_OBJECTS_H

#include "DatabaseModuleDragDrop.h"

namespace DATABASE
{
    class DatabaseMgr;
}

class MapEditorWidgetObjectMapping;

class MapEditorModuleWorldObjects : public DatabaseModuleTooltipList
{
private:
    QWidget* MapEditorModuleWorldObjects::_setupTooltipWidget(uint32 uiPrototypeID);

protected:
    void hideEvent(QHideEvent* pEvent);
    void showEvent(QShowEvent* pEvent);

public:
    MapEditorModuleWorldObjects(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);

    const MapEditorWidgetObjectMapping* getToolWidget() const { return m_pObjectWidget; }

private:
    const DATABASE::DatabaseMgr& m_DBMgr;
    MapEditorWidgetObjectMapping* m_pObjectWidget = nullptr;
};

#endif
