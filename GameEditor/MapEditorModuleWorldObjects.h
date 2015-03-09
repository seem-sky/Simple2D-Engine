#ifndef MAP_EDITOR_MODULE_WORLD_OBJECTS_H
#define MAP_EDITOR_MODULE_WORLD_OBJECTS_H

#include "TooltipList.h"

namespace database
{
    class Manager;
}

class MapEditorWidgetObjectMapping;

class MapEditorModuleWorldObjects : public TooltipList
{
private:
    QWidget* _setupTooltipWidget(uint32 uiPrototypeID);

protected:
    void hideEvent(QHideEvent* pEvent);
    void showEvent(QShowEvent* pEvent);

public:
    MapEditorModuleWorldObjects(const database::Manager& DBMgr, QWidget* pParent = nullptr);

    const MapEditorWidgetObjectMapping* getToolWidget() const { return m_pObjectWidget; }

private:
    const database::Manager& m_DBMgr;
    MapEditorWidgetObjectMapping* m_pObjectWidget = nullptr;
};
#endif
