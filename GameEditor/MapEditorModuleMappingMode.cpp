#include "MapEditorModuleMappingMode.h"
#include "moc_MapEditorModuleMappingMode.h"
#include "MapEditorModuleWorldObjects.h"
#include "MapEditorModuleTileSelection.h"
#include "MapEditorModuleScriptArea.h"
#include "MapEditorWidgetObjectMapping.h"
#include <QtWidgets/QGridLayout>

MapEditorModuleMappingMode::MapEditorModuleMappingMode(CACHE::Manager& cacheMgr, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent)
    : QWidget(pParent), m_DBMgr(DBMgr), m_CacheMgr(cacheMgr),
    m_pModuleTileSelection(new MapEditorModuleTileSelection(cacheMgr, DBMgr, this)),
    m_pModuleWorldObjects(new MapEditorModuleWorldObjects(DBMgr, this)),
    m_pModuleScriptArea(new MapEditorModuleScriptArea(this)),

    // others
    m_pMappingMode(new QComboBox(this)),
    m_MappingObject(m_DBMgr, this)
{
    // setup mapping mode QComboBox
    m_pMappingMode->addItems(m_MappingObject.getMappingModeNames());
    connect(m_pMappingMode, SIGNAL(currentIndexChanged(int)), this, SLOT(_onMappingModeChanged(int)));
    m_pMappingMode->setCurrentIndex(static_cast<int>(m_MappingObject.getMappingModeType()));
    m_pMappingMode->show();
    _onMappingModeChanged(m_pMappingMode->currentIndex());

    // connect Tiles
    connect(m_pModuleTileSelection->getBrushWidget(), SIGNAL(changeBrushInfo(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo)),
        m_MappingObject.getMappingMode(MAPPING_MODE::Type::TILE_MAPPING), SLOT(onBrushInfoChanged(BRUSH::BrushIndex, MAP::BRUSH::BrushInfo)));

    // connect WorldObject
    connect(m_pModuleWorldObjects, SIGNAL(changeIndex(int32)), this, SLOT(_onWorldObjectIndexChanged(int32)));
    connect(m_pModuleWorldObjects->getToolWidget(), SIGNAL(changeDirection(MAP::MAP_DATA::MapDirection)),
        m_MappingObject.getMappingMode(MAPPING_MODE::Type::OBJECT_MAPPING), SLOT(onDirectionChanged(MAP::MAP_DATA::MapDirection)));
    connect(this, SIGNAL(changeWorldObjectID(uint32)), m_MappingObject.getMappingMode(MAPPING_MODE::Type::OBJECT_MAPPING), SLOT(onIDChanged(uint32)));

    // setup layout
    auto pLayout = new QGridLayout(this);
    setLayout(pLayout);
    pLayout->setRowStretch(1, 1);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(m_pMappingMode, 0, 0);
    pLayout->addWidget(m_pModuleTileSelection, 1, 0);
    pLayout->addWidget(m_pModuleWorldObjects, 1, 0);
}

void MapEditorModuleMappingMode::setup()
{
    m_pModuleTileSelection->clearSelection();
    m_pModuleTileSelection->setup();

    // setup ModuleWorldObjects
    m_pModuleWorldObjects->setModel(new DATABASE::ConstDatabaseModel(m_DBMgr, DATABASE::DatabaseType::WORLD_OBJECT_DATABASE));
}

const MappingObject& MapEditorModuleMappingMode::getMappingMode() const
{
    return m_MappingObject;
}

void MapEditorModuleMappingMode::_onMappingModeChanged(int index)
{
    auto mode = static_cast<MAPPING_MODE::Type>(index);

    // first hide all
    m_pModuleTileSelection->hide();
    m_pModuleWorldObjects->hide();
    m_pModuleScriptArea->hide();

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
        m_pModuleScriptArea->show();
        break;

    case MAPPING_MODE::Type::PRESENTATION:
        break;
    default:
        throw std::runtime_error("invalid mapping mode detected");
    }

    m_MappingObject.setMappingMode(mode);
    emit changeMappingMode(mode);
}

void MapEditorModuleMappingMode::_onWorldObjectIndexChanged(int32 index)
{
    emit changeWorldObjectID(index < 0 ? 0 : index + 1);
}

