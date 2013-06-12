#include "ModifyView.h"
#include "moc_ModifyView.h"
#include <QtGui/QKeyEvent>

ModifyItem::ModifyItem() : QGraphicsPixmapItem()
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

void ModifyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsPixmapItem::mouseReleaseEvent(pEvent);
    move(0, 0);
    sendUpdate();
}

void ModifyItem::keyPressEvent(QKeyEvent *pEvent)
{
    switch(pEvent->key())
    {
    case Qt::Key_Up: move(0, -1); break;
    case Qt::Key_Down: move(0, 1); break;
    case Qt::Key_Left: move(-1, 0); break;
    case Qt::Key_Right: move(1, 0); break;
    case Qt::Key_Delete:
        if (QGraphicsScene *pScene = scene())
        {
            sendUpdate();
            pScene->removeItem(this);
        }
        delete this;
    default: return;
    }
    sendUpdate();
}

void ModifyItem::sendUpdate()
{
    if (ModifyView *pWidget = dynamic_cast<ModifyView*>(scene()->parent()))
        pWidget->itemChanged(this);
}

void ModifyItem::move(int32 x, int32 y)
{
    moveBy(x, y);
}

void ModifyView::itemChanged(ModifyItem *pItem)
{
    emit onItemChange(pItem);
}