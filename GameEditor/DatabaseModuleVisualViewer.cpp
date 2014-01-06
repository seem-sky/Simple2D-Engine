#include "DatabaseModuleVisualViewer.h"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSizePolicy>
#include <QtGui/QResizeEvent>
#include <QtWidgets/QHeaderView>

/*#####
# VisualViewerWidget
#####*/
VisualViewerItem::VisualViewerItem(const DATABASE::DatabaseMgr& DBMgr, QWidget *pParent) : QFrame(pParent),
    // widgets
    m_pVisualViewer(new VisualViewer(DBMgr, this))
{
    // setup layout
    auto pLayout = new QHBoxLayout(this);
    setLayout(pLayout);
    pLayout->setContentsMargins(0, 0, 0, 0);

    pLayout->addWidget(m_pVisualViewer);

    // setup frame
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setLineWidth(1);
    setMidLineWidth(1);
    setContentsMargins(2, 2, 2, 2);
}

/*#####
# DatabaseModuleVisualViewer
#####*/
DatabaseModuleVisualViewer::DatabaseModuleVisualViewer(QWidget* pParent) : QWidget(pParent), m_pDBMgr(nullptr),
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
    m_pVisualViewerList->verticalHeader()->setVisible(false);
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
}

void DatabaseModuleVisualViewer::setDatabaseMgr(const DATABASE::DatabaseMgr* pDBMgr)
{
    m_pDBMgr = pDBMgr;
}

void DatabaseModuleVisualViewer::insertVisualViewer(uint32 index)
{
    if (!m_pDBMgr)
        assert("You have to setup m_pDBMgr first.");

    m_pVisualViewerList->insertRow(index);
    m_pVisualViewerList->setCellWidget(index, 0, new VisualViewerItem(*m_pDBMgr, m_pVisualViewerList));
    m_pVisualViewerList->setRowHeight(index, 180);
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
        for (uint32 i = oldCount; i <= count; ++i)
            insertVisualViewer(i);
    }
    // remove
    else if (count < oldCount)
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
