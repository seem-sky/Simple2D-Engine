#include "AbstractPrototypeTable.h"
#include "moc_AbstractPrototypeTable.h"
#include <QtWidgets/QHeaderView>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

/*#####
# AbstractPixmapWidget
#####*/
AbstractPixmapWidget::AbstractPixmapWidget(uint32 ID, QWidget* pParent) : QWidget(pParent), m_Selection(0), m_ID(ID)
{
    resize(MAP::TILE_SIZE, MAP::TILE_SIZE);
}

void AbstractPixmapWidget::addSelection(Selection selection)
{
    if (!hasSelection(selection))
    {
        m_Selection |= selection;
        repaint();
    }
}

bool AbstractPixmapWidget::hasSelection(Selection selection)
{
    return (m_Selection&  selection) != 0;
}

void AbstractPixmapWidget::removeSelection(Selection selection)
{
    if (hasSelection(selection))
    {
        m_Selection ^= selection;
        repaint();
    }
}

void AbstractPixmapWidget::paintEvent(QPaintEvent* pEvent)
{
    QWidget::paintEvent(pEvent);

    drawPixmap();
    drawSelection();
}

void AbstractPixmapWidget::drawSelection()
{
    QPainter painter(this);
    const uint32 SIZE = MAP::TILE_SIZE - 1;

    QPen pen;
    pen.setWidth(5);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    if (m_Selection&  SELECTION_RIGHT && m_Selection&  SELECTION_LEFT)
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
    else if (m_Selection&  SELECTION_RIGHT)
    {
        QPen pen = painter.pen();
        pen.setColor(Qt::yellow);
        painter.setPen(pen);
        painter.drawRect(0, 0, SIZE, SIZE);
    }
    else if (m_Selection&  SELECTION_LEFT)
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
AbstractPrototypeTable::AbstractPrototypeTable(const database::Manager& DBMgr, QWidget* pParent)
    : QTableWidget(pParent), m_DBMgr(DBMgr)
{
    // setup header
    auto hHeader = horizontalHeader();
    hHeader->setDefaultSectionSize(MAP::TILE_SIZE + 1);
    hHeader->setSectionResizeMode(QHeaderView::Fixed);
    hHeader->setVisible(false);
    auto vHeader = verticalHeader();
    vHeader->setDefaultSectionSize(MAP::TILE_SIZE + 1);
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
}

void AbstractPrototypeTable::mousePressEvent(QMouseEvent* pEvent)
{
    if (auto pWidget = dynamic_cast<AbstractPixmapWidget*>(cellWidget(rowAt(pEvent->y()), columnAt(pEvent->x()))))
    {
        if (pEvent->button() == Qt::RightButton || pEvent->button() == Qt::LeftButton)
        {
            auto brush = BRUSH::BrushIndex::BRUSH_LEFT;
            if (pEvent->button() == Qt::RightButton)
                brush = BRUSH::BrushIndex::BRUSH_RIGHT;

            clickItem(brush, pWidget);
        }
    }
    else
        QTableWidget::mousePressEvent(pEvent);
}

void AbstractPrototypeTable::clickItem(BRUSH::BrushIndex brush, AbstractPixmapWidget* pItem)
{
    emit selectionChanged(brush, getType(), pItem->getID());
    emit itemClicked(brush, pItem);
}

void AbstractPrototypeTable::showEvent(QShowEvent* pEvent)
{
    // if has no data, reload it
    if (!columnCount() && !rowCount())
        _setup();
}

void AbstractPrototypeTable::setup()
{
    if (isVisible())
        _setup();
}
