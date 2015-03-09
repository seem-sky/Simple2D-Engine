#include "DatabaseModulePrototypeAnimationAdditionType.h"
#include "DatabaseModulePrototypeBase.h"
#include <Database/Prototype/Derived.h>

using namespace database;
using namespace ui::module;

PrototypeAnimationAdditionType::PrototypeAnimationAdditionType(QWidget* pParent)
    : PrototypeInterface(pParent), m_pModuleBase(new PrototypeBase(this))
{
    auto pLayout = new QGridLayout();
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(m_pModuleBase);
    setLayout(pLayout);
}

void PrototypeAnimationAdditionType::setup(const prototype::AnimationAdditionType* pPrototype)
{
    m_pModuleBase->setup(pPrototype);
}

void PrototypeAnimationAdditionType::setupFrom(prototype::AnimationAdditionType* pPrototype) const
{
    m_pModuleBase->setupFrom(pPrototype);
}

void PrototypeAnimationAdditionType::clear()
{
    m_pModuleBase->clear();
}
