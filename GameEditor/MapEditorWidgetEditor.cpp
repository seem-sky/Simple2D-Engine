#include "MapEditorWidgetEditor.h"
#include "moc_MapEditorWidgetEditor.h"

const std::array<const char*, 3> MAPPING_MODES =
{
    "tile mapping",
    "object mapping",
    "presentation"
};

MapEditorWidgetEditor::MapEditorWidgetEditor(DATABASE::DatabaseMgr& databaseMgr, QWidget* pParent) : QWidget(pParent), m_DatabaseMgr(databaseMgr),
    // modules
    m_pModuleTileSelection(new MapEditorModuleTileSelection(m_DatabaseMgr, this)),
    m_pModuleMapTree(new MapEditorModuleMapTree(m_DatabaseMgr, pParent)),
    m_pModuleContent(new MapEditorModuleContent(m_DatabaseMgr, this)),
    m_pModuleWorldObjects(new MapEditorModuleWorldObjects(m_DatabaseMgr, this)),

    // others
    m_pMappingMode(new QComboBox(this))
{
    // setup mapping mode QComboBox
    for (uint32 i = 0; i < 3; ++i)
        m_pMappingMode->addItem(MAPPING_MODES[i]);
    connect(m_pMappingMode, SIGNAL(currentIndexChanged(int)), this, SLOT(_onMappingModeChanged(int)));
    m_pMappingMode->setCurrentIndex(static_cast<int>(MappingMode::TILE_MAPPING));
    m_pMappingMode->show();
    _onMappingModeChanged(m_pMappingMode->currentIndex());

    // setup modules
    auto pLayout = new QGridLayout();
    setLayout(pLayout);
    pLayout->setContentsMargins(0, 0, 0, 0);

    pLayout->setColumnMinimumWidth(0, 200);
    pLayout->setRowMinimumHeight(1, 200);
    pLayout->setColumnStretch(1, 0);
    pLayout->setColumnStretch(1, 1);
    pLayout->setRowStretch(1, 1);

    pLayout->addWidget(m_pMappingMode, 0, 0);
    pLayout->addWidget(m_pModuleWorldObjects, 1, 0);
    pLayout->addWidget(m_pModuleTileSelection, 1, 0);
    pLayout->addWidget(m_pModuleMapTree, 2, 0);
    pLayout->addWidget(m_pModuleContent, 0, 1, -1, -1);

    connect(m_pModuleMapTree, SIGNAL(openMap(uint32)), m_pModuleContent, SLOT(onMapOpened(uint32)));
    connect(m_pModuleMapTree, SIGNAL(editMap(uint32)), m_pModuleContent, SLOT(onMapEdited(uint32)));

    connect(m_pModuleContent, SIGNAL(registerTab(MapViewer*)), this, SLOT(onRegisterTab(MapViewer*)));
}

void MapEditorWidgetEditor::_onMappingModeChanged(int index)
{
    MappingMode mode = MappingMode::PRESENTATION;

    // first hide all
    m_pModuleTileSelection->getBrushWidget()->hide();
    m_pModuleTileSelection->hide();

    m_pModuleWorldObjects->hide();

    // show only the needed widgets
    switch (static_cast<MappingMode>(index))
    {
    case MappingMode::TILE_MAPPING:
        m_pModuleTileSelection->show();
        m_pModuleTileSelection->getBrushWidget()->show();

        mode = MappingMode::TILE_MAPPING;
        break;

    case MappingMode::OBJECT_MAPPING:
        m_pModuleWorldObjects->show();

        mode = MappingMode::OBJECT_MAPPING;
        break;

    case MappingMode::PRESENTATION:

        mode = MappingMode::PRESENTATION;
        break;
    default:
        throw std::runtime_error("invalid mapping mode detected");
    }

    m_pModuleContent->setMappingMode(mode);
}

void MapEditorWidgetEditor::setup()
{
    m_pModuleTileSelection->clearSelection();
    m_pModuleTileSelection->setup();

    // setup ModuleWorldObjects
    m_pModuleWorldObjects->setModel(new DATABASE::ConstDatabaseModel(m_DatabaseMgr, DATABASE::DatabaseType::WORLD_OBJECT_DATABASE));
}

void MapEditorWidgetEditor::projectOpened()
{
    setup();
    m_pModuleMapTree->reload();
}

void MapEditorWidgetEditor::onRegisterTab(MapViewer* pTab)
{
    connect(pTab, SIGNAL(requestBrushInfo(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo&)),
        m_pModuleTileSelection->getBrushWidget(), SLOT(onBrushInfoRequested(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo&)));
    pTab->setMappingMode(static_cast<MappingMode>(m_pMappingMode->currentIndex()));
}
