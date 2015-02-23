#include "MapEditorWidgetObjectMapping.h"
#include "moc_MapEditorWidgetObjectMapping.h"
#include <DatabaseMgr.h>

MapEditorWidgetObjectMapping::MapEditorWidgetObjectMapping(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) :
Ui_MapEditorWidgetObjectMapping(), ToolWidget(pParent), m_DBMgr(DBMgr)
{
    setupUi(this);

    setWindowTitle("map editor world objects");

    connect(m_pDirectionLEFT, SIGNAL(clicked()), this, SLOT(_onDirectionChanged()));
    connect(m_pDirectionUP, SIGNAL(clicked()), this, SLOT(_onDirectionChanged()));
    connect(m_pDirectionRIGHT, SIGNAL(clicked()), this, SLOT(_onDirectionChanged()));
    connect(m_pDirectionDOWN, SIGNAL(clicked()), this, SLOT(_onDirectionChanged()));

    m_pViewer->setDatabaseMgr(&DBMgr);
    m_pViewer->showGrid(false);
}

MAP::MAP_DATA::MapDirection MapEditorWidgetObjectMapping::_getDirection() const
{
    if (m_pDirectionDOWN->isChecked())
        return MAP::MAP_DATA::MapDirection::down;
    if (m_pDirectionUP->isChecked())
        return MAP::MAP_DATA::MapDirection::up;
    if (m_pDirectionRIGHT->isChecked())
        return MAP::MAP_DATA::MapDirection::right;
    return MAP::MAP_DATA::MapDirection::left;
}

void MapEditorWidgetObjectMapping::_onDirectionChanged()
{
    emit changeDirection(_getDirection());
    _setupViewer();
}

void MapEditorWidgetObjectMapping::onIndexChanged(int32 index)
{
    m_WorldObjectID = index < 0 ? 0 : index + 1;
    _setupViewer();
}

void MapEditorWidgetObjectMapping::_setupViewer()
{
    m_pViewer->clear();
    if (auto pPrototype = m_DBMgr.getWorldObjectDatabase()->getOriginalPrototype(m_WorldObjectID))
    {
        if (pPrototype->getFlagManager().hasFlag(DATABASE::PROTOTYPE::WORLD_OBJECT::Flags::FLAG_ANIMATION))
        {
            auto index = static_cast<uint32>(_getDirection())+1;
            for (uint32 i = 0; i < pPrototype->getAnimationModule().getAnimationCount(); ++i)
            {
                auto& info = pPrototype->getAnimationModule().getAnimationInfo(i);
                if (info.m_AnimationTypeID == index)
                    m_pViewer->setAnimation(info);
            }
        }
    }
}
