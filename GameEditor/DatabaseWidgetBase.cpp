#include "DatabaseWidgetBase.h"
#include "moc_DatabaseWidgetBase.h"

DatabaseWidgetBase::DatabaseWidgetBase(QWidget *pParent) : QWidget(pParent), m_pModuleList(new DatabaseModuleList(this))
{
    // setup layout
    QGridLayout *pLayout(new QGridLayout(this));
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
    if (m_pModuleList->getDataID())
    {
        if (auto pDB = getDatabase())
        {
            auto pPrototype = pDB->getNewPrototype();
            setupPrototypeFromWidgets(pPrototype);
            pDB->setPrototype(pPrototype);
        }
    }
    //clear widgets
    clear();
    // load widgets from prototype
    if (auto pPrototype = m_pModuleList->getCurrentPrototype())
        setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetBase::setupWidgetsFromPrototype(const DATABASE::Prototype *pPrototype)
{
    m_pModuleList->setDataName(pPrototype->getName());
    m_pModuleList->setDataID(pPrototype->getID());
}

void DatabaseWidgetBase::setupPrototypeFromWidgets(DATABASE::Prototype *pPrototype)
{
    pPrototype->setName(m_pModuleList->getDataName());
    pPrototype->setID(m_pModuleList->getDataID());
}

void DatabaseWidgetBase::clear()
{
    m_pModuleList->setDataID(0);
    m_pModuleList->setDataName("");
}