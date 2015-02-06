#include "MapEditorWidgetEditor.h"
#include "moc_MapEditorWidgetEditor.h"
#include "MapEditor.h"
#include <QtWidgets/QComboBox>
#include <WorldObjectInfo.h>
#include "MapEditorWidgetObjectMapping.h"

MapEditorWidgetEditor::MapEditorWidgetEditor(DATABASE::DatabaseMgr& databaseMgr, QWidget* pParent) : QWidget(pParent), m_DatabaseMgr(databaseMgr), m_CacheMgr(databaseMgr),
    // modules
    m_pModuleTileSelection(new MapEditorModuleTileSelection(m_CacheMgr, m_DatabaseMgr, this)),
    m_pModuleMapTree(new MapEditorModuleMapTree(m_DatabaseMgr, pParent)),
    m_pModuleContent(new MapEditorModuleContent(m_CacheMgr, m_MappingObject, m_DatabaseMgr, this)),
    m_pModuleWorldObjects(new MapEditorModuleWorldObjects(m_DatabaseMgr, this)),

    // others
    m_pMappingMode(new QComboBox(this)),
    m_MappingObject(databaseMgr, this)
{
    // setup mapping mode QComboBox
    m_pMappingMode->addItems(m_MappingObject.getMappingModeNames());
    connect(m_pMappingMode, SIGNAL(currentIndexChanged(int)), this, SLOT(_onMappingModeChanged(int)));
    m_pMappingMode->setCurrentIndex(static_cast<int>(m_MappingObject.getMappingModeType()));
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

    connect(this, SIGNAL(changeMappingMode(MAPPING_MODE::Type)), m_pModuleContent, SLOT(onMappingModeChanged(MAPPING_MODE::Type)));

    // connect Tiles
    connect(m_pModuleTileSelection->getBrushWidget(), SIGNAL(changeBrushInfo(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo)),
        m_MappingObject.getMappingMode(MAPPING_MODE::Type::TILE_MAPPING), SLOT(onBrushInfoChanged(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo)));

    // connect WorldObject
    connect(m_pModuleWorldObjects, SIGNAL(changeIndex(int32)), this, SLOT(onWorldObjectIndexChanged(int32)));
    connect(m_pModuleWorldObjects->getWorldObjectWidget(), SIGNAL(changeDirection(MAP::MAP_DATA::MapDirection)),
        m_MappingObject.getMappingMode(MAPPING_MODE::Type::OBJECT_MAPPING), SLOT(onDirectionChanged(MAP::MAP_DATA::MapDirection)));
    connect(this, SIGNAL(changeWorldObjectID(uint32)),
        m_MappingObject.getMappingMode(MAPPING_MODE::Type::OBJECT_MAPPING), SLOT(onIDChanged(uint32)));
}

void MapEditorWidgetEditor::_onMappingModeChanged(int index)
{
    auto mode = static_cast<MAPPING_MODE::Type>(index);

    // first hide all
    m_pModuleTileSelection->hide();
    m_pModuleWorldObjects->hide();

    // show only the needed widgets
    switch (mode)
    {
    case MAPPING_MODE::Type::TILE_MAPPING:
        m_pModuleTileSelection->show();
        break;

    case MAPPING_MODE::Type::OBJECT_MAPPING:
        m_pModuleWorldObjects->show();
        break;

    case MAPPING_MODE::Type::SCRIPT_AREA_MAPPING:
    case MAPPING_MODE::Type::PRESENTATION:
        break;
    default:
        throw std::runtime_error("invalid mapping mode detected");
    }

    m_MappingObject.setMappingMode(mode);
    emit changeMappingMode(mode);
}

void MapEditorWidgetEditor::setup()
{
    // clear cache first
    m_CacheMgr.clear();

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

void MapEditorWidgetEditor::onWorldObjectIndexChanged(int32 index)
{
    emit changeWorldObjectID(index < 0 ? 0 : index+1);
}

void MapEditorWidgetEditor::onSaveChanges()
{
    m_pModuleContent->saveMaps();
}
