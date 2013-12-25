#ifndef MAP_EDITOR_MODULE_TILE_SELECTION_H
#define MAP_EDITOR_MODULE_TILE_SELECTION_H

#include "DatabaseMgr.h"
#include <QtWidgets/QTabWidget>
#include "MapEditorModuleTiles.h"
#include "MapEditorWidgetBrush.h"
#include "MapEditorModuleTileSets.h"

class MapEditorModuleTileSelection : public QTabWidget
{
public:
    MapEditorModuleTileSelection(const DATABASE::DatabaseMgr &databaseMgr, QWidget *pParent = nullptr);

    void setup();

private:
    const DATABASE::DatabaseMgr &m_DatabaseMgr;

    // tabs
    MapEditorModuleTiles *m_pModuleTiles;
    MapEditorModuleAutoTiles *m_pModuleAutoTiles;
    MapEditorModuleTileSets *m_pModuleTileSets;

    BRUSH::MapEditorWidgetBrush *m_pWidgetBrush;
};

#endif
