#include "MapEditorWidgetEditor.h"
#include "moc_MapEditorWidgetEditor.h"
#include "MapEditorModuleMappingMode.h"
#include "Config.h"

MapEditorWidgetEditor::MapEditorWidgetEditor(database::Manager& DBMgr, QWidget* pParent)
    : QWidget(pParent), m_CacheMgr(DBMgr), m_DBMgr(DBMgr),
    // modules
    m_pModuleMapTree(new MapEditorModuleMapTree(DBMgr.getMapDatabase(), pParent)),
    m_pModuleContent(new MapEditorModuleContent(m_CacheMgr, m_pModuleMappingMode->getMappingMode(), DBMgr, this)),
    m_pModuleMappingMode(new MapEditorModuleMappingMode(m_CacheMgr, DBMgr, this))
{
    // setup modules
    auto pLayout = new QGridLayout();
    setLayout(pLayout);
    pLayout->setContentsMargins(0, 0, 0, 0);

    pLayout->setColumnMinimumWidth(0, 200);
    pLayout->setRowMinimumHeight(1, 200);
    pLayout->setColumnStretch(1, 0);
    pLayout->setColumnStretch(1, 1);
    pLayout->setRowStretch(1, 1);

    //pLayout->addWidget(m_pMappingMode, 0, 0);
    pLayout->addWidget(m_pModuleMappingMode, 0, 0);
    pLayout->addWidget(m_pModuleMapTree, 1, 0);
    pLayout->addWidget(m_pModuleContent, 0, 1, -1, -1);

    connect(m_pModuleMapTree, SIGNAL(openMap(uint32)), m_pModuleContent, SLOT(onMapOpened(uint32)));
    connect(m_pModuleMapTree, SIGNAL(editMap(uint32)), m_pModuleContent, SLOT(onMapEdited(uint32)));

    connect(m_pModuleMappingMode, SIGNAL(changeMappingMode(MAPPING_MODE::Type)), m_pModuleContent, SLOT(onMappingModeChanged(MAPPING_MODE::Type)));
}

void MapEditorWidgetEditor::setup()
{
    // clear cache first
    m_CacheMgr.clear();
    m_pModuleMappingMode->setup();
}

void MapEditorWidgetEditor::projectOpened()
{
    setup();
    m_pModuleMapTree->reload();
}

void MapEditorWidgetEditor::onSaveChanges()
{
    m_pModuleContent->saveMaps();
    m_pModuleMapTree->save(Config::get()->getProjectDirectory());
}
