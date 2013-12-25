#ifndef DATABASE_MODULE_TILE_SETS_H
#define DATABASE_MODULE_TILE_SETS_H

#include "DatabaseModuleDragDrop.h"
#include "MapEditorWidgetBrush.h"

class MapEditorModuleTileSets : public DatabaseModuleDragList
{
    Q_OBJECT
private:
    QWidget* _setupTooltipWidget(uint32 uiPrototypeID);

protected:
    void mousePressEvent(QMouseEvent* pEvent);

public:
    MapEditorModuleTileSets(QWidget* pParent = nullptr);

    void setTileDatabase(const DATABASE::TileDatabase* pDB);

    void setModel(IDatabaseModel* pModel);

signals:
    void selectionChanged(BRUSH::BrushIndex brush, MAP::BRUSH::SelectionType selectioType, uint32 ID);
    void itemClicked(BRUSH::BrushIndex brush);

private:
    const DATABASE::TileDatabase* m_pTileDB;
};

#endif
