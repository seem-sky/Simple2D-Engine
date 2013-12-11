#include "AbstractPrototypeTable.h"
#include "moc_AbstractPrototypeTable.h"
#include <QtWidgets/QHeaderView>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

/*#####
# TexturePrototypeFrameEx
#####*/
TexturePrototypeFrameEx::TexturePrototypeFrameEx(QWidget *pParent) : TexturePrototypeFrame(pParent), m_Selection(0)
{
    resize(TILE_SIZE, TILE_SIZE);
}

void TexturePrototypeFrameEx::addSelection(Selection selection)
{
    if (!hasSelection(selection))
    {
        m_Selection |= selection;
        repaint();
    }
}

bool TexturePrototypeFrameEx::hasSelection(Selection selection)
{
    return (m_Selection & selection) != 0;
}

void TexturePrototypeFrameEx::removeSelection(Selection selection)
{
    if (hasSelection(selection))
    {
        m_Selection ^= selection;
        repaint();
    }
}

void TexturePrototypeFrameEx::paintEvent(QPaintEvent *pEvent)
{
    TexturePrototypeFrame::paintEvent(pEvent);

    QPainter painter(this);
    const uint32 SIZE = TILE_SIZE-1;

    QPen pen;
    pen.setWidth(5);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    if (m_Selection & SELECTION_RIGHT && m_Selection & SELECTION_LEFT)
    {
        // draw left
        QPen pen = painter.pen();
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawLine(0, 0, 0, SIZE);
        painter.drawLine(0, 0, SIZE, 0);

        // draw right
        pen = painter.pen();
        pen.setColor(Qt::yellow);
        painter.setPen(pen);
        painter.drawLine(SIZE, 0, SIZE, SIZE);
        painter.drawLine(0, SIZE, SIZE, SIZE);
    }
    else if (m_Selection & SELECTION_RIGHT)
    {
        QPen pen = painter.pen();
        pen.setColor(Qt::yellow);
        painter.setPen(pen);
        painter.drawRect(0, 0, SIZE, SIZE);
    }
    else if (m_Selection & SELECTION_LEFT)
    {
        QPen pen = painter.pen();
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawRect(0, 0, SIZE, SIZE);
    }
}

/*#####
# MapEditorModuleTiles
#####*/
AbstractPrototypeTable::AbstractPrototypeTable(QWidget *pParent) : QTableWidget(pParent), m_pDBMgr(nullptr)
{
    // setup header
    auto hHeader = horizontalHeader();
    hHeader->setDefaultSectionSize(TILE_SIZE+1);
    hHeader->setSectionResizeMode(QHeaderView::Fixed);
    hHeader->setVisible(false);
    auto vHeader = verticalHeader();
    vHeader->setDefaultSectionSize(TILE_SIZE+1);
    vHeader->setSectionResizeMode(QHeaderView::Fixed);
    vHeader->setVisible(false);

    // table widget
    setSelectionMode(QAbstractItemView::NoSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setTabKeyNavigation(false);
    setFrameShape(QFrame::NoFrame);
}

void AbstractPrototypeTable::clear()
{
    setColumnCount(0);
    setRowCount(0);
    m_SelectedItems.fill(nullptr);
}

void AbstractPrototypeTable::mousePressEvent(QMouseEvent *pEvent)
{
    if (auto pWidget = dynamic_cast<TexturePrototypeFrameEx*>(cellWidget(rowAt(pEvent->y()), columnAt(pEvent->x()))))
    {
        if (pEvent->button() == Qt::RightButton)
            _itemRightClicked(pWidget);
        else if (pEvent->button() == Qt::LeftButton)
            _itemLeftClicked(pWidget);
    }
    else
        QTableWidget::mousePressEvent(pEvent);
}

void AbstractPrototypeTable::showEvent(QShowEvent *pEvent)
{
    // if has no data, reload it
    if (!columnCount() && !rowCount())
        _setup();
}

void AbstractPrototypeTable::_itemLeftClicked(TexturePrototypeFrameEx *pItem)
{
    if (m_SelectedItems.at(LEFT))
        m_SelectedItems.at(LEFT)->removeSelection(TexturePrototypeFrameEx::SELECTION_LEFT);
    pItem->addSelection(TexturePrototypeFrameEx::SELECTION_LEFT);
    m_SelectedItems.at(LEFT) = pItem;
    emit itemLeftClicked(pItem);
}

void AbstractPrototypeTable::_itemRightClicked(TexturePrototypeFrameEx *pItem)
{
    if (m_SelectedItems.at(RIGHT))
        m_SelectedItems.at(RIGHT)->removeSelection(TexturePrototypeFrameEx::SELECTION_RIGHT);
    pItem->addSelection(TexturePrototypeFrameEx::SELECTION_RIGHT);
    m_SelectedItems.at(RIGHT) = pItem;
    emit itemRightClicked(pItem);
}

void AbstractPrototypeTable::setup()
{
    if (isVisible())
        _setup();
}
