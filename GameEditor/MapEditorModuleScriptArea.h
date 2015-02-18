#ifndef MAP_EDITOR_MODULE_SCRIPT_AREA_H
#define MAP_EDITOR_MODULE_SCRIPT_AREA_H

#include <QtWidgets/QWidget>
#include "MapEditorWidgetScriptArea.h"

class MapEditorModuleScriptArea : public QWidget
{
protected:
    void hideEvent(QHideEvent* pEvent);
    void showEvent(QShowEvent* pEvent);

public:
    MapEditorModuleScriptArea(QWidget* pParent = nullptr);

private:
    MapEditorWidgetScriptArea* m_pWidgetScriptArea;
};
#endif
