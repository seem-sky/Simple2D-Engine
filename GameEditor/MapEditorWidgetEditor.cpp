#include "MapEditorWidgetEditor.h"
#include "moc_MapEditorWidgetEditor.h"

MapEditorWidgetEditor::MapEditorWidgetEditor(DATABASE::DatabaseMgr &databaseMgr, QWidget *pParent) : QWidget(pParent), m_DatabaseMgr(databaseMgr),
    // modules
    m_pModuleTileSelection(new MapEditorModuleTileSelection(m_DatabaseMgr, this)),
    m_pModuleMapTree(new MapEditorModuleMapTree(pParent)),
    m_pModuleContent(new MapEditorModuleContent(m_DatabaseMgr, this))
{
    // setup modules
    auto pLayout = new QGridLayout();
    setLayout(pLayout);
    pLayout->setContentsMargins(0, 0, 0, 0);

    pLayout->setColumnMinimumWidth(0, 200);
    pLayout->setRowMinimumHeight(1, 200);
    pLayout->setColumnStretch(1, 0);
    pLayout->setColumnStretch(1, 1);

    pLayout->addWidget(m_pModuleTileSelection, 0, 0);
    pLayout->addWidget(m_pModuleMapTree, 1, 0);
    pLayout->addWidget(m_pModuleContent, 0, 1, -1, -1);

    connect(m_pModuleMapTree, SIGNAL(openMap(uint32)), m_pModuleContent, SLOT(onMapOpened(uint32)));
    connect(m_pModuleMapTree, SIGNAL(editMap(uint32)), m_pModuleContent, SLOT(onMapEdited(uint32)));
}

void MapEditorWidgetEditor::setup()
{
    m_pModuleTileSelection->setup();
}

void MapEditorWidgetEditor::projectOpened()
{
    setup();
    m_pModuleMapTree->setDatabase(m_DatabaseMgr.getMapDatabase());
}
