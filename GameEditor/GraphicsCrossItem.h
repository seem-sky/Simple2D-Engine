#pragma once

#include <QtGui/QPen>
#include <QtGlobal.h>

class GraphicsCrossItem : public QGraphicsItem
{
public:
    GraphicsCrossItem(QGraphicsItem* pItem = nullptr);

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);
    QRectF boundingRect() const;

    const QPen& getPen() const;
    void setPen(const QPen& pen);

    void setCrossSize(const QSize& size);
    void setCrossSize(int width, int height);
    QSize getCrossSize() const;

private:
    DiagonalCross m_Cross;
};
