#include "DatabaseWidgetBase.h"
#include "moc_DatabaseWidgetBase.h"

using namespace DATABASE;

DatabaseWidgetBase::DatabaseWidgetBase(QWidget* pParent) : QWidget(pParent), m_pModuleList(new DatabaseModuleList(this))
{
    setFocusPolicy(Qt::StrongFocus);

    // setup layout
    QGridLayout* pLayout(new QGridLayout(this));
    setLayout(pLayout);
    layout()->setContentsMargins(5, 5, 5, 5);
    pLayout->setHorizontalSpacing(15);
    pLayout->setColumnStretch(1, 1);

    // add database list
    pLayout->addWidget(m_pModuleList, 0, 0, -1, 1);

    // setup database content
    connect(m_pModuleList, SIGNAL(selectionChanged()), this, SLOT(_onItemSelected()));
}

void DatabaseWidgetBase::_onItemSelected()
{
    // save previous prototype first
    saveCurrent();
    //clear widgets
    clear();
    // load widgets from prototype
    if (auto pPrototype = m_pModuleList->getCurrentPrototype())
        setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetBase::setupWidgetsFromPrototype(const Prototype* pPrototype)
{
    m_pModuleList->setDataName(pPrototype->getName());
    m_pModuleList->setDataID(pPrototype->getID());
}

void DatabaseWidgetBase::setupPrototypeFromWidgets(Prototype* pPrototype)
{
    pPrototype->setName(m_pModuleList->getDataName());
    pPrototype->setID(m_pModuleList->getDataID());
}

void DatabaseWidgetBase::clear()
{
    m_pModuleList->setDataID(0);
    m_pModuleList->setDataName("");
}

void DatabaseWidgetBase::saveCurrent()
{
    if (m_pModuleList->getDataID())
    {
        if (auto pModel = m_pModuleList->getDatabaseModel())
        {
            if (auto pDB = pModel->getDatabase())
            {
                auto pPrototype = pDB->getNewPrototype();
                setupPrototypeFromWidgets(pPrototype);
                pDB->setPrototype(pPrototype);
            }
        }
    }
}

void DatabaseWidgetBase::hideEvent(QHideEvent* pEvent)
{
    saveCurrent();
}

ConstDatabaseModel* DatabaseWidgetBase::getDatabaseModel() const
{
    return dynamic_cast<ConstDatabaseModel*>(m_pModuleList->getDatabaseModel());
}
