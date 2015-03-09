#include "DatabaseModulePrototypeBase.h"
#include <Database/Prototype/Base.h>

using namespace database;
using namespace ui::module;

PrototypeBase::PrototypeBase(QWidget* pParent)
    : PrototypeInterface(pParent), Ui_DatabaseModulePrototypeBase()
{
    setupUi(this);
}

void PrototypeBase::setup(const prototype::Base* pPrototype)
{
    if (!pPrototype)
        throw std::bad_typeid("nullptr");

    m_pID->setValue(pPrototype->getID());
    m_pName->setText(pPrototype->getName());
}

void PrototypeBase::setupFrom(prototype::Base* pPrototype) const
{
    if (!pPrototype)
        throw std::bad_typeid("nullptr");

    pPrototype->setName(m_pName->text());
}

void PrototypeBase::clear()
{
    m_pID->clear();
    m_pName->clear();
}
