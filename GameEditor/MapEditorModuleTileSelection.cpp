#include "MapEditorModuleTileSelection.h"
#include "moc_MapEditorModuleTileSelection.h"

MapEditorModuleTileSelection::MapEditorModuleTileSelection(CACHE::Manager& cacheMgr, const DATABASE::DatabaseMgr& databaseMgr, QWidget* pParent)
    : QTabWidget(pParent), m_CacheMgr(cacheMgr), m_DatabaseMgr(databaseMgr), m_pWidgetBrush(new BRUSH::MapEditorWidgetBrush(m_DatabaseMgr, this)),
    // tabs
    m_pModuleTiles(new MapEditorModuleTiles(m_CacheMgr, m_DatabaseMgr, this)),
    m_pModuleAutoTiles(new MapEditorModuleAutoTiles(m_DatabaseMgr, this)),
    m_pModuleTileSets(new MapEditorModuleTileSets(pParent))
{
    m_SelectedItems.fill(nullptr);

    // setup tile modules
    addTab(m_pModuleTiles, "tiles");
    addTab(m_pModuleAutoTiles, "auto tiles");
    addTab(m_pModuleTileSets, "tile sets");
    setCurrentIndex(0);

    setMaximumWidth(200);
    m_pModuleTiles->setMaximumWidth(maximumWidth());
    m_pModuleAutoTiles->setMaximumWidth(maximumWidth());
    m_pModuleTileSets->setMaximumWidth(maximumWidth());

    // connect brush with tile selection
    connect(m_pModuleTiles, SIGNAL(selectionChanged(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo::Type, uint32)),
        m_pWidgetBrush, SLOT(onSelectionChanged(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo::Type, uint32)));
    connect(m_pModuleAutoTiles, SIGNAL(selectionChanged(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo::Type, uint32)),
        m_pWidgetBrush, SLOT(onSelectionChanged(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo::Type, uint32)));
    connect(m_pModuleTileSets, SIGNAL(selectionChanged(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo::Type, uint32)),
        m_pWidgetBrush, SLOT(onSelectionChanged(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo::Type, uint32)));

    connect(m_pModuleTiles, SIGNAL(itemClicked(BRUSH::BrushIndex, AbstractPixmapWidget*)),
        this, SLOT(onItemClicked(BRUSH::BrushIndex, AbstractPixmapWidget*)));
    connect(m_pModuleAutoTiles, SIGNAL(itemClicked(BRUSH::BrushIndex, AbstractPixmapWidget*)),
        this, SLOT(onItemClicked(BRUSH::BrushIndex, AbstractPixmapWidget*)));
    connect(m_pModuleTileSets, SIGNAL(itemClicked(BRUSH::BrushIndex)),
        this, SLOT(onItemClicked(BRUSH::BrushIndex)));

    m_pWidgetBrush->show();
}

void MapEditorModuleTileSelection::clearSelection()
{
    m_SelectedItems.fill(nullptr);
    m_pWidgetBrush->clear();
}

void MapEditorModuleTileSelection::setup()
{
    m_SelectedItems.fill(nullptr);

    m_pModuleTiles->setup();
    m_pModuleAutoTiles->setup();

    m_pModuleTileSets->setModel(new DATABASE::ConstDatabaseModel(m_DatabaseMgr, DATABASE::DatabaseType::TILE_SET_DATABASE));
}

void MapEditorModuleTileSelection::onItemClicked(BRUSH::BrushIndex brush, AbstractPixmapWidget* pWidget)
{
    // move selection from previous selected widget to new one;
    // items in m_pModuleTileSets doesn´t have a AbstractPixmapWidget, so only store nullptr
    auto selection = AbstractPixmapWidget::SELECTION_LEFT;
    if (brush == BRUSH::BrushIndex::BRUSH_RIGHT)
        selection = AbstractPixmapWidget::SELECTION_RIGHT;

    if (m_SelectedItems.at(static_cast<uint32>(brush)))
        m_SelectedItems.at(static_cast<uint32>(brush))->removeSelection(selection);
    if (pWidget)
        pWidget->addSelection(selection);
    m_SelectedItems.at(static_cast<uint32>(brush)) = pWidget;
}

void MapEditorModuleTileSelection::hideEvent(QHideEvent* pEvent)
{
    m_pWidgetBrush->hide();
    QWidget::hideEvent(pEvent);
}

void MapEditorModuleTileSelection::showEvent(QShowEvent* pEvent)
{
    m_pWidgetBrush->show();
    QWidget::showEvent(pEvent);
}
