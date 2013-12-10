#include "MapEditorModuleTiles.h"
#include <QtWidgets/QHeaderView>
#include <QtGui/QMouseEvent>
#include "moc_MapEditorModuleTiles.h"
#include <QtGui/QPainter>

/*#####
# TexturePrototypeFrameEx
#####*/
TexturePrototypeFrameEx::TexturePrototypeFrameEx(QWidget *pParent) : TexturePrototypeFrame(pParent), m_pTexturePrototype(nullptr), m_Selection(0)
{
    resize(TILE_SIZE, TILE_SIZE);
}

void TexturePrototypeFrameEx::setTexturePrototype(const DATABASE::TexturePrototype *pPrototype)
{
    m_pTexturePrototype = pPrototype;
    repaint();
}

const DATABASE::TexturePrototype* TexturePrototypeFrameEx::getTexturePrototype() const
{
    return m_pTexturePrototype;
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
    if (m_Selection & RIGHT && m_Selection & LEFT)
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
    else if (m_Selection & RIGHT)
    {
        QPen pen = painter.pen();
        pen.setColor(Qt::yellow);
        painter.setPen(pen);
        painter.drawRect(0, 0, SIZE, SIZE);
    }
    else if (m_Selection & LEFT)
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
MapEditorModuleTiles::MapEditorModuleTiles(QWidget *pParent) : QTableWidget(pParent)
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

    // selection
    setSelectionMode(QAbstractItemView::NoSelection);
}

void MapEditorModuleTiles::setup(const DATABASE::TileDatabase *pTileDB)
{
    clear();
    if (!pTileDB)
        return;

    const uint32 tilesPerRow = width()/(TILE_SIZE+2);
    setColumnCount(tilesPerRow);
    setRowCount(std::ceil(double(pTileDB->getSize())/tilesPerRow));
    for (uint32 i = 0; i < pTileDB->getSize(); ++i)
    {
        auto pTileFrame = new TexturePrototypeFrameEx(this);
        if (auto pPrototype = pTileDB->getOriginalPrototype(i))
            pTileFrame->setTexturePrototype(pPrototype);
        setCellWidget(i/tilesPerRow, i%tilesPerRow, pTileFrame);
    }
}

void MapEditorModuleTiles::clear()
{
    setColumnCount(0);
    setRowCount(0);
    m_SelectedItems.fill(nullptr);
}

void MapEditorModuleTiles::mousePressEvent(QMouseEvent *pEvent)
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

void MapEditorModuleTiles::_itemLeftClicked(TexturePrototypeFrameEx *pItem)
{
    if (m_SelectedItems.at(0))
        m_SelectedItems.at(0)->removeSelection(TexturePrototypeFrameEx::LEFT);
    pItem->addSelection(TexturePrototypeFrameEx::LEFT);
    m_SelectedItems.at(0) = pItem;
    emit itemLeftClicked(pItem);
}

void MapEditorModuleTiles::_itemRightClicked(TexturePrototypeFrameEx *pItem)
{
    if (m_SelectedItems.at(1))
        m_SelectedItems.at(1)->removeSelection(TexturePrototypeFrameEx::RIGHT);
    pItem->addSelection(TexturePrototypeFrameEx::RIGHT);
    m_SelectedItems.at(1) = pItem;
    emit itemRightClicked(pItem);
}
