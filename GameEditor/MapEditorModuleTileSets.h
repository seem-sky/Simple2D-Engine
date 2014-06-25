#ifndef DATABASE_MODULE_TILE_SETS_H
#define DATABASE_MODULE_TILE_SETS_H

#include "DatabaseModuleDragDrop.h"
#include "MapEditorWidgetBrush.h"

class MapEditorModuleTileSets : public DatabaseModuleTooltipList
{
    Q_OBJECT
private:
    QWidget* _setupTooltipWidget(uint32 uiPrototypeID);

protected:
    void mousePressEvent(QMouseEvent* pEvent);

public:
    MapEditorModuleTileSets(QWidget* pParent = nullptr);

signals:
    void selectionChanged(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo::Type type, uint32 ID);
    void itemClicked(BRUSH::BrushIndex brush);
};

#endif
