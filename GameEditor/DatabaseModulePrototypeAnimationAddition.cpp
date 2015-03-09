#include "DatabaseModulePrototypeAnimationAddition.h"
#include "moc_DatabaseModulePrototypeAnimationAddition.h"
#include "DatabaseModulePrototypeTexture.h"
#include <Database/Prototype/Derived.h>

using namespace database;
using namespace ui::module;

PrototypeAnimationAddition::PrototypeAnimationAddition(animation::addition::EditorScene* pScene, const helper::CreatorInterface& creator, database::Model* pTypeModel,
    QWidget* pParent)
    : PrototypeInterface(pParent), m_pScene(pScene), Ui_DatabaseModulePrototypeAnimationAddition()
{
    setupUi(this);

    // setup scene
    m_pView->setScene(m_pScene);
    connect(m_pScene, SIGNAL(selectionDataChanged(const animation::SpriteData&)), m_pModuleSelected, SLOT(onSelectedChanged(const animation::SpriteData&)));
    connect(m_pScene, SIGNAL(selectionCleared()), m_pModuleSelected, SLOT(onSelectionCleared()));
    connect(m_pModuleSelected, SIGNAL(positionChanged(const QPoint&)), m_pScene, SLOT(onPositionChanged(const QPoint&)));
    connect(m_pModuleSelected, SIGNAL(zValueChanged(qreal)), m_pScene, SLOT(onZValueChanged(qreal)));
    connect(m_pModuleSelected, SIGNAL(scaleChanged(qreal)), m_pScene, SLOT(onScaleChanged(qreal)));
    connect(m_pModuleSelected, SIGNAL(opacityChanged(qreal)), m_pScene, SLOT(onOpacityChanged(qreal)));
    connect(m_pModuleSelected, SIGNAL(rotationChanged(qreal)), m_pScene, SLOT(onRotationChanged(qreal)));

    m_pModuleSpriteList->setup(creator);

    // setup combobox
    m_pTypeBox->setModel(pTypeModel);
    m_pTypeBox->setView(new List(pTypeModel, this));
    m_pTypeBox->setModelColumn(1);

    connect(m_pZoom, SIGNAL(valueChanged(int)), this, SLOT(_onZoomChanged(int)));
}

void PrototypeAnimationAddition::setup(const prototype::AnimationAddition* pPrototype)
{
    m_pModuleBase->setup(pPrototype);

    m_pTypeBox->setCurrentIndex(pPrototype->getAdditionType() - 1);
    m_pScene->setup(pPrototype);
}

void PrototypeAnimationAddition::setupFrom(prototype::AnimationAddition* pPrototype) const
{
    m_pModuleBase->setupFrom(pPrototype);

    pPrototype->setAdditionType(m_pTypeBox->currentIndex() + 1);
    m_pScene->setup(pPrototype);
}

void PrototypeAnimationAddition::clear()
{
    m_pModuleBase->clear();

    m_pTypeBox->setCurrentIndex(-1);
    m_pScene->clearContents();
    m_pZoom->setValue(100);
}

void PrototypeAnimationAddition::_onZoomChanged(int value)
{
    m_pView->resetTransform();
    m_pView->scale(qreal(value) / 100, qreal(value) / 100);
}
