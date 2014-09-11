#include "DatabaseModuleVisualViewer.h"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSizePolicy>
#include <QtGui/QResizeEvent>
#include <QtWidgets/QHeaderView>
#include "moc_DatabaseModuleVisualViewer.h"

using namespace DATABASE::PROTOTYPE;
using namespace DATABASE::PROTOTYPE::WORLD_OBJECT;

/*#####
# VisualViewerWidget
#####*/
VisualViewerItem::VisualViewerItem(QWidget *pParent, bool standardEntry) : QFrame(pParent), m_TypeID(1),
    // widgets
    m_pVisualViewer(new VisualViewer(this)),
    m_pAnimationType(new QComboBox(this))
{
    // setup layout
    auto pLayout = new QVBoxLayout(this);
    setLayout(pLayout);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(0);

    pLayout->addWidget(m_pAnimationType);
    pLayout->addWidget(m_pVisualViewer);

    // setup frame
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setLineWidth(1);
    setMidLineWidth(1);
    setContentsMargins(2, 2, 2, 2);

    m_pAnimationType->setEnabled(!standardEntry);
}

void VisualViewerItem::setAnimation(uint32 ID, MODULE::ANIMATION::VisualType type)
{
    m_pVisualViewer->setAnimation(ID, type);
}

void VisualViewerItem::setAnimationType(uint32 ID)
{
    m_pAnimationType->setCurrentIndex(ID-1);
    m_TypeID = ID;
}

void VisualViewerItem::setAnimationTypeModel(DATABASE::ConstDatabaseModel* pModel)
{
    m_pAnimationType->setModel(pModel);
    m_pAnimationType->setModelColumn(1);
    m_pAnimationType->setCurrentIndex(int32(m_TypeID)-1);
}

/*#####
# DatabaseModuleVisualViewer
#####*/
DatabaseModuleVisualViewer::DatabaseModuleVisualViewer(QWidget* pParent) : QWidget(pParent), m_pDBMgr(nullptr), m_pAnimationTypeModel(nullptr),
    // widgets
    m_pVisualViewerList(new QTableWidget(this)),
    m_pAddButton(new QPushButton("+", this)),
    m_pRemoveButton(new QPushButton("-", this))
{
    // setup buttons
    m_pAddButton->setMaximumWidth(23);
    m_pRemoveButton->setMaximumWidth(23);

    // setup table
    m_pVisualViewerList->horizontalHeader()->setStretchLastSection(true);
    m_pVisualViewerList->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_pVisualViewerList->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_pVisualViewerList->horizontalHeader()->setVisible(false);
    m_pVisualViewerList->setColumnCount(1);
    m_pVisualViewerList->setSelectionMode(QAbstractItemView::NoSelection);

    // setup layout
    auto pLayout = new QGridLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    pLayout->setColumnStretch(0, 1);
    pLayout->setColumnStretch(1, 0);
    pLayout->setRowStretch(0, 0);
    pLayout->setRowStretch(1, 0);
    pLayout->setRowStretch(2, 1);

    pLayout->addWidget(m_pVisualViewerList, 0, 0, -1, 1);
    pLayout->addWidget(m_pRemoveButton, 0, 1, 1, 1);
    pLayout->addWidget(m_pAddButton, 1, 1, 1, 1);

    connect(m_pAddButton, SIGNAL(clicked()), this, SLOT(_onAddButtonClicked()));
    connect(m_pRemoveButton, SIGNAL(clicked()), this, SLOT(_onRemoveButtonClicked()));
}

void DatabaseModuleVisualViewer::setDatabaseMgr(const DATABASE::DatabaseMgr* pDBMgr)
{
    m_pDBMgr = pDBMgr;
    m_pAnimationTypeModel = new DATABASE::ConstDatabaseModel(*m_pDBMgr, DATABASE::DatabaseType::ANIMATION_TYPE_DATABASE);
    for (int32 row = 0; row < m_pVisualViewerList->rowCount(); ++row)
    {
        if (auto pViewerItem = dynamic_cast<VisualViewerItem*>(m_pVisualViewerList->cellWidget(row, 0)))
        {
            pViewerItem->setAnimationTypeModel(m_pAnimationTypeModel);
            if (auto pViewer = pViewerItem->getVisualViewer())
                pViewer->setDatabaseMgr(pDBMgr);
        }
    }
}

void DatabaseModuleVisualViewer::insertVisualViewer(uint32 index)
{
    m_pVisualViewerList->insertRow(index);
    m_pVisualViewerList->setRowHeight(index, 180);
    auto pViewer = new VisualViewerItem(m_pVisualViewerList, index < MIN_WORLD_OBJECT_POSE);
    m_pVisualViewerList->setCellWidget(index, 0, pViewer);
    pViewer->getVisualViewer()->setDatabaseMgr(m_pDBMgr);
    if (m_pAnimationTypeModel)
        pViewer->setAnimationTypeModel(m_pAnimationTypeModel);
}

void DatabaseModuleVisualViewer::removeVisualViewer(uint32 index)
{
    m_pVisualViewerList->removeRow(index);
}

void DatabaseModuleVisualViewer::setVisualViewerCount(uint32 count)
{
    const auto oldCount = getVisualViewerCount();
    if (count == oldCount)
        return;

    // add
    if (count > oldCount)
    {
        for (uint32 i = oldCount; i < count; ++i)
            insertVisualViewer(i);
    }
    // remove
    else
    {
        do 
        {
            removeVisualViewer(count);
        } while (count > getVisualViewerCount());
    }
}

uint32 DatabaseModuleVisualViewer::getVisualViewerCount() const
{
    return m_pVisualViewerList->rowCount();
}

void DatabaseModuleVisualViewer::_onAddButtonClicked()
{
    insertVisualViewer(getVisualViewerCount());
}

void DatabaseModuleVisualViewer::_onRemoveButtonClicked()
{
    removeVisualViewer(getVisualViewerCount()-1);
}

void DatabaseModuleVisualViewer::clear()
{
    m_pVisualViewerList->setRowCount(0);
}

VisualViewerItem* DatabaseModuleVisualViewer::getVisualViewer(uint32 index) const
{
    return dynamic_cast<VisualViewerItem*>(m_pVisualViewerList->cellWidget(index, 0));
}
