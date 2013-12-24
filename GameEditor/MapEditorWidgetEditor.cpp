#include "MapEditorWidgetEditor.h"
#include "moc_MapEditorWidgetEditor.h"


MapEditorWidgetEditor::MapEditorWidgetEditor(DATABASE::DatabaseMgr &databaseMgr, QWidget *pParent) : QWidget(pParent), Ui_MapEditorWidget(), m_DatabaseMgr(databaseMgr),
    m_pContent(nullptr)
{
    setupUi(this);

    // setup content
    m_pContent = new MapEditorModuleContent(m_DatabaseMgr, this);
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->setColumnMinimumWidth(0, 200);
        pLayout->setRowMinimumHeight(1, 200);
        pLayout->addWidget(m_pContent, 0, 1, -1, -1);
    }

    connect(m_pMapTree, SIGNAL(openMap(uint32)), m_pContent, SLOT(onMapOpened(uint32)));
    connect(m_pMapTree, SIGNAL(editMap(uint32)), m_pContent, SLOT(onMapEdited(uint32)));
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

void MapEditorWidgetEditor::projectOpened()
{
    setup();
    m_pMapTree->setDatabase(m_DatabaseMgr.getMapDatabase());
}
