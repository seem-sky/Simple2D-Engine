#ifndef MAP_EDITOR_MODULE_TILE_SELECTION_H
#define MAP_EDITOR_MODULE_TILE_SELECTION_H

#include "DatabaseMgr.h"
#include <QtWidgets/QTabWidget>
#include "MapEditorModuleTiles.h"
#include "MapEditorWidgetBrush.h"
#include "MapEditorModuleTileSets.h"

namespace CACHE
{
    class Manager;
}

class MapEditorModuleTileSelection : public QTabWidget
{
    Q_OBJECT
protected:
    void hideEvent(QHideEvent* pEvent);
    void showEvent(QShowEvent* pEvent);

public:
    MapEditorModuleTileSelection(CACHE::Manager& cacheMgr, const DATABASE::DatabaseMgr& databaseMgr, QWidget* pParent = nullptr);

    void setup();

    void clearSelection();

    inline BRUSH::MapEditorWidgetBrush* getBrushWidget() const { return m_pWidgetBrush; }

public slots:
    void onItemClicked(BRUSH::BrushIndex brush, AbstractPixmapWidget* pWidget = nullptr);

private:
    std::array<AbstractPixmapWidget*, 2> m_SelectedItems;
    const DATABASE::DatabaseMgr& m_DatabaseMgr;
    CACHE::Manager& m_CacheMgr;

    // tabs
    MapEditorModuleTiles* m_pModuleTiles;
    MapEditorModuleAutoTiles* m_pModuleAutoTiles;
    MapEditorModuleTileSets* m_pModuleTileSets;

    BRUSH::MapEditorWidgetBrush* m_pWidgetBrush;
};

#endif
