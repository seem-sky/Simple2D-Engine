#include "MapEditorModuleTileSelection.h"

MapEditorModuleTileSelection::MapEditorModuleTileSelection(const DATABASE::DatabaseMgr &databaseMgr, QWidget *pParent) : QTabWidget(pParent),
    m_DatabaseMgr(databaseMgr), m_pWidgetBrush(new BRUSH::MapEditorWidgetBrush(m_DatabaseMgr, this)),
    // tabs
    m_pModuleTiles(new MapEditorModuleTiles(m_DatabaseMgr, this)),
    m_pModuleAutoTiles(new MapEditorModuleAutoTiles(m_DatabaseMgr, this)),
    m_pModuleTileSets(new MapEditorModuleTileSets(pParent))
{
    // setup tile modules
    addTab(m_pModuleTiles, "tiles");
    addTab(m_pModuleAutoTiles, "auto tiles");
    addTab(m_pModuleTileSets, "tile sets");
    setCurrentIndex(0);

    setMaximumWidth(200);

    // connect brush with tile selection
    connect(m_pModuleTiles, SIGNAL(selectionChanged(BRUSH::BrushIndex, BRUSH::SelectionType, uint32)),
        m_pWidgetBrush, SLOT(_onSelectionChanged(BRUSH::BrushIndex, BRUSH::SelectionType, uint32)));
    connect(m_pModuleAutoTiles, SIGNAL(selectionChanged(BRUSH::BrushIndex, BRUSH::SelectionType, uint32)),
        m_pWidgetBrush, SLOT(_onSelectionChanged(BRUSH::BrushIndex, BRUSH::SelectionType, uint32)));

    m_pWidgetBrush->show();
}

void MapEditorModuleTileSelection::setup()
{
    m_pModuleTiles->setup();
    m_pModuleAutoTiles->setup();

    // ToDo: hacky solution, find better in future
    m_pModuleTileSets->setModel(new TileSetDatabaseModel(std::unique_ptr<DATABASE::TileSetDatabase>(
        const_cast<DATABASE::TileSetDatabase*>(m_DatabaseMgr.getTileSetDatabase()))));
    m_pModuleTileSets->setTileDatabase(m_DatabaseMgr.getTileDatabase());
}
