#include "DatabaseModulePrototypeAnimation.h"
#include "moc_DatabaseModulePrototypeAnimation.h"
#include "DatabaseModulePrototypeBase.h"
#include <Database/Prototype/Derived.h>

using namespace database;
using namespace ui::module;

PrototypeAnimation::PrototypeAnimation(const helper::CreatorInterface& creator, database::Model* pAdditionTypeModel, QWidget* pParent)
    : PrototypeInterface(pParent), Ui_DatabaseModulePrototypeAnimation()
{
    setupUi(this);

    m_pModuleSpriteList->setup(creator);

    m_pModuleSelection->setAdditionTypeModel(pAdditionTypeModel);

    m_pScene = new animation::EditorScene(creator, m_pView);
    m_pView->setScene(m_pScene);
    connect(m_pModuleFrameNavigation, SIGNAL(saveFrame(uint32, uint32)), this, SLOT(_onSaveCurrentFrame(uint32, uint32)));
    connect(m_pModuleFrameNavigation, SIGNAL(frameSelectionChanged(const FrameData&)), m_pScene, SLOT(setup(const FrameData&)));

    connect(m_pScene, SIGNAL(selectionDataChanged(const animation::SpriteData&)), m_pModuleSelection, SLOT(onSelectedChanged(const animation::SpriteData&)));
    connect(m_pScene, SIGNAL(selectionDataChanged(const animation::AdditionData&)), m_pModuleSelection, SLOT(onSelectedChanged(const animation::AdditionData&)));
    connect(m_pScene, SIGNAL(selectionCleared()), m_pModuleSelection, SLOT(onSelectionCleared()));
    connect(m_pAddPointAddition, SIGNAL(clicked()), m_pScene, SLOT(onPointAdditionAdd()));
    connect(m_pAddRectAddition, SIGNAL(clicked()), m_pScene, SLOT(onRectAdditionAdd()));

    connect(m_pModuleSelection->getModuleSelectedAddition(), SIGNAL(positionChanged(const QPoint&)), m_pScene, SLOT(onPositionChanged(const QPoint&)));
    connect(m_pModuleSelection->getModuleSelectedAddition(), SIGNAL(sizeChanged(const QSize&)), m_pScene, SLOT(onSizeChanged(const QSize&)));
    connect(m_pModuleSelection->getModuleSelectedAddition(), SIGNAL(zValueChanged(qreal)), m_pScene, SLOT(onZValueChanged(qreal)));
    connect(m_pModuleSelection->getModuleSelectedAddition(), SIGNAL(scaleChanged(qreal)), m_pScene, SLOT(onScaleChanged(qreal)));
    connect(m_pModuleSelection->getModuleSelectedAddition(), SIGNAL(opacityChanged(qreal)), m_pScene, SLOT(onOpacityChanged(qreal)));
    connect(m_pModuleSelection->getModuleSelectedAddition(), SIGNAL(rotationChanged(qreal)), m_pScene, SLOT(onRotationChanged(qreal)));
    connect(m_pModuleSelection->getModuleSelectedAddition(), SIGNAL(additionTypeIDChanged(uint32)), m_pScene, SLOT(onAdditionTypeIDChanged(uint32)));

    connect(m_pModuleSelection->getModuleSelectedSprite(), SIGNAL(positionChanged(const QPoint&)), m_pScene, SLOT(onPositionChanged(const QPoint&)));
    connect(m_pModuleSelection->getModuleSelectedSprite(), SIGNAL(zValueChanged(qreal)), m_pScene, SLOT(onZValueChanged(qreal)));
    connect(m_pModuleSelection->getModuleSelectedSprite(), SIGNAL(scaleChanged(qreal)), m_pScene, SLOT(onScaleChanged(qreal)));
    connect(m_pModuleSelection->getModuleSelectedSprite(), SIGNAL(opacityChanged(qreal)), m_pScene, SLOT(onOpacityChanged(qreal)));
    connect(m_pModuleSelection->getModuleSelectedSprite(), SIGNAL(rotationChanged(qreal)), m_pScene, SLOT(onRotationChanged(qreal)));
}

void PrototypeAnimation::setup(const prototype::Animation* pPrototype)
{
    m_pModuleBase->setup(pPrototype);

    m_Frames = pPrototype->getFrames();
    m_pModuleFrameNavigation->setup(m_Frames);
}

void PrototypeAnimation::setupFrom(prototype::Animation* pPrototype) const
{
    m_pModuleBase->setupFrom(pPrototype);

    m_pModuleFrameNavigation->saveCurrent();
    pPrototype->setFrames(m_Frames);
}

void PrototypeAnimation::clear()
{
    m_pModuleBase->clear();

    m_pModuleFrameNavigation->clear();
    m_Frames.clear();
    m_Frames.resize(1);
}

void PrototypeAnimation::_onSaveCurrentFrame(uint32 index, uint32 time)
{
    if (index < m_Frames.size())
    {
        m_Frames.at(index) = m_pScene->getFrame();
        m_Frames.at(index).setTime(time);
    }
}
