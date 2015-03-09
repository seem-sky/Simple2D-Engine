#include "PrototypeTooltipList.h"

PrototypeTooltipList::PrototypeTooltipList(QWidget* pParent)
    : TooltipList(pParent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragEnabled(true);
    setToolTipPosition(PrototypeTooltipList::ToolTipPosition::Left);
    setShowTooltip(true);
}

PrototypeTooltipList::PrototypeTooltipList(const helper::CreatorInterface& creator, QWidget* pParent)
    : PrototypeTooltipList(pParent)
{
    setup(creator);
}

void PrototypeTooltipList::setup(const helper::CreatorInterface& creator)
{
    m_pCreator = &creator;
    setModel(new database::Model(creator.getDatabase(), this));
    resizeColumnToContents(0);
}


void PrototypeTooltipList::startDrag(Qt::DropActions supportedActions)
{
    auto item = currentIndex();
    if (!item.isValid())
        QTreeView::startDrag(supportedActions);

    auto pDrag = new QDrag(this);
    auto mimeData = new QMimeData();
    if (m_pCreator)
        pDrag->setPixmap(m_pCreator->createPixmap(item.row() + 1));
    mimeData->setText(QString::number(item.row() + 1));
    pDrag->setMimeData(mimeData);
    pDrag->exec(supportedActions);
}

QWidget* PrototypeTooltipList::_setupTooltipWidget(uint32 uiPrototypeID)
{
    auto pLabel = new QLabel(this);
    if (m_pCreator)
    {
        auto pixmap = m_pCreator->createPixmap(uiPrototypeID);
        pLabel->setPixmap(pixmap);
        pLabel->resize(pixmap.size());
    }
    return pLabel;
}
