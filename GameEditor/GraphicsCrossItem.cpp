#include "GraphicsCrossItem.h"
#include <QtGui/QPainter>
#include "QtGlobal.h"

GraphicsCrossItem::GraphicsCrossItem(QGraphicsItem* pItem)
    : QGraphicsItem(pItem)
{
    setFlag(QGraphicsItem::ItemIgnoresParentOpacity);

    QPen newPen;
    newPen.setWidth(1);
    newPen.setColor(Qt::black);
    setPen(newPen);
    setCrossSize(5, 5);
}

QSize GraphicsCrossItem::getCrossSize() const
{
    return m_Cross.getCrossSize();
}

void GraphicsCrossItem::setCrossSize(const QSize& size)
{
    m_Cross.setCrossSize(size);
    prepareGeometryChange();
}

void GraphicsCrossItem::setCrossSize(int width, int height)
{
    setCrossSize(QSize(width, height));
}

const QPen& GraphicsCrossItem::getPen() const
{
    return m_Cross.getPen();
}

void GraphicsCrossItem::setPen(const QPen& pen)
{
    m_Cross.setPen(pen);
    update();
    prepareGeometryChange();
}

QRectF GraphicsCrossItem::boundingRect() const
{
    return m_Cross.boundingRect();
}

void GraphicsCrossItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    pPainter->setPen(m_Cross.getPen());
    m_Cross.paint(pPainter);

    if (pOption->state & QStyle::State_Selected)
        highlightSelection(*this, pPainter, pOption);
}
