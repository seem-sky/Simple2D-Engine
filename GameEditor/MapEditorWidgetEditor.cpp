#include "MapEditorWidgetEditor.h"
#include "moc_MapEditorWidgetEditor.h"

MapEditorWidgetEditor::MapEditorWidgetEditor(DATABASE::DatabaseMgr &databaseMgr, QWidget *pParent) : QWidget(pParent), Ui_MapEditorWidget(), m_DatabaseMgr(databaseMgr)
{
    setupUi(this);
}

void MapEditorWidgetEditor::_setupTileModules()
{
    m_pModuleTiles->setDBMgr(&m_DatabaseMgr);
    m_pModuleTiles->setup();

    m_pModuleAutoTiles->setDBMgr(&m_DatabaseMgr);
    m_pModuleAutoTiles->setup();

    m_pModuleTileSets->setModel(new TileSetDatabaseModel(std::unique_ptr<DATABASE::TileSetDatabase>(m_DatabaseMgr.getTileSetDatabase())));
    m_pModuleTileSets->setTileDatabase(m_DatabaseMgr.getTileDatabase());
}

void MapEditorWidgetEditor::setup()
{
    _setupTileModules();
}

