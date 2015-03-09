#include "GraphicsTextureItem.h"
#include "DelayedDeleteObject.h"
#include "QtGlobal.h"
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOption>
#include <QtGlobal.h>
#include "Config.h"

/*#####
# GraphicsTextureItem
#####*/
GraphicsSpriteItem::GraphicsSpriteItem(uint32 ID) : QGraphicsItem(), m_ID(ID)
{
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
        highlightSelection(*this, pPainter, pOption);
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
