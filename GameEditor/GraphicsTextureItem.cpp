#include "GraphicsTextureItem.h"
#include "DelayedDeleteObject.h"
#include "QtGlobal.h"
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOption>
#include "Config.h"

/*#####
# GraphicsTextureItem
#####*/
GraphicsSpriteItem::GraphicsSpriteItem(uint32 ID) : QGraphicsItem(), m_ID(ID)
{
    if (!m_ID)
        assert("ID not allowed to be NULL!");
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

void GraphicsSpriteItem::keyPressEvent(QKeyEvent* pEvent)
{
    switch(pEvent->key())
    {
    case Qt::Key_Up: moveBy(0, -1); break;
    case Qt::Key_Down: moveBy(0, 1); break;
    case Qt::Key_Left: moveBy(-1, 0); break;
    case Qt::Key_Right: moveBy(1, 0); break;
    }
}

void GraphicsSpriteItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    auto pixmap = _getPixmap();
    setTransformOriginPoint(pixmap.rect().center());
    pPainter->drawPixmap(0, 0, pixmap);
    // draw selection box
    if (isSelected())
        _hightlightSelection(pPainter, pOption);
}

void GraphicsSpriteItem::_hightlightSelection(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption)
{
    // from QGraphicsItem.cpp
    const QRectF murect = pPainter->transform().mapRect(QRectF(0, 0, 1, 1));
    if (qFuzzyCompare(qMax(murect.width(), murect.height()) + 1, 1))
        return;

    const QRectF mbrect = pPainter->transform().mapRect(boundingRect());
    if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
        return;

    qreal itemPenWidth = 1.0;
    const qreal pad = itemPenWidth / 2;

    const qreal penWidth = 0; // cosmetic pen

    const QColor fgcolor = pOption->palette.windowText().color();
    const QColor bgcolor( // ensure good contrast against fgcolor
        fgcolor.red()   > 127 ? 0 : 255,
        fgcolor.green() > 127 ? 0 : 255,
        fgcolor.blue()  > 127 ? 0 : 255);

    pPainter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
    pPainter->setBrush(Qt::NoBrush);
    pPainter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));

    pPainter->setPen(QPen(pOption->palette.windowText(), 0, Qt::DashLine));
    pPainter->setBrush(Qt::NoBrush);
    pPainter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));
}

QRectF GraphicsSpriteItem::boundingRect() const
{
    auto pixmap = _getPixmap();
    return pixmap.rect();
}

uint32 GraphicsSpriteItem::getID() const
{
    return m_ID;
}
