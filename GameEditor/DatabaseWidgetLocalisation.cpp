#include "DatabaseWidgetLocalisation.h"

using namespace DATABASE::LOCALISATION;

DatabaseWidgetLocalisation::DatabaseWidgetLocalisation(QWidget *pParent) : DatabaseWidgetBase(pParent), m_pModuleLocalisation(new DatabaseModuleLocalisation(this))
{
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
        pLayout->addWidget(m_pModuleLocalisation, 0, 1, -1, 1);
}

void DatabaseWidgetLocalisation::setupWidgetsFromPrototype(const DATABASE::Prototype *pPrototype)
{
    if (auto pProto = dynamic_cast<const LocalisationPrototype*>(pPrototype))
    {
        for (uint32 i = 0; i < pProto->getLocalsCount() && i < LOCALISATION_COUNT; ++i)
            m_pModuleLocalisation->setLocalisation(i, pProto->getLocalisation(i));
    }
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetLocalisation::setupPrototypeFromWidgets(DATABASE::Prototype *pPrototype)
{
    if (auto pProto = dynamic_cast<LocalisationPrototype*>(pPrototype))
    {
        for (uint32 i = 0; i < LOCALISATION_COUNT; ++i)
            pProto->setLocalisation(i, m_pModuleLocalisation->getLocalisation(i));
    }
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetLocalisation::clear()
{
    m_pModuleLocalisation->clear();
    DatabaseWidgetBase::clear();
}
