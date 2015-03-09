#include "DatabaseModulePrototypeAnimationAdditionPoint.h"
#include <QtWidgets/QGridLayout>
#include <Database/Prototype/Derived.h>

using namespace database;
using namespace ui::module;

PrototypeAnimationAdditionPoint::PrototypeAnimationAdditionPoint(const helper::CreatorInterface& creator, database::Model* pTypeModel, QWidget* pParent)
    : PrototypeInterface(pParent), m_pModuleAddition(new PrototypeAnimationAddition(new animation::addition::PointEditorScene(creator, pParent), creator, pTypeModel))
{
    auto pLayout = new QGridLayout();
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(m_pModuleAddition);
    setLayout(pLayout);
}

void PrototypeAnimationAdditionPoint::setup(const prototype::AnimationAdditionPoint* pPrototype)
{
    m_pModuleAddition->setup(pPrototype);
}

void PrototypeAnimationAdditionPoint::setupFrom(prototype::AnimationAdditionPoint* pPrototype) const
{
    m_pModuleAddition->setupFrom(pPrototype);
}

void PrototypeAnimationAdditionPoint::clear()
{
    m_pModuleAddition->clear();
}
