#include "MapViewItem.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsScene>

MapViewItem::MapViewItem(MAP::MAP_DATA::WorldObjectInfo& info, const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap), m_WorldObjectInfo(info)
{
    setFlag(ItemSendsScenePositionChanges);
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
}

QPoint MapViewItem::_checkMove(QPoint moveTo) const
{
    //// calculate positions
    //QRect rect;
    //if (x() < moveTo.x())
    //{
    //    rect.setX(x() + getWorldObjectBoundingRect().getPositionX());
    //    rect.setWidth(moveTo.x() + getWorldObjectBoundingRect().getPositionX());
    //}
    //else
    //{
    //    rect.setWidth(x() + getWorldObjectBoundingRect().getPositionX());
    //    rect.setX(moveTo.x() + getWorldObjectBoundingRect().getPositionX());
    //}

    //if (y() < moveTo.y())
    //{
    //    rect.setY(x() + getWorldObjectBoundingRect().getPositionY());
    //    rect.setHeight(moveTo.x() + getWorldObjectBoundingRect().getPositionY());
    //}
    //else
    //{
    //    rect.setHeight(x() + getWorldObjectBoundingRect().getPositionY());
    //    rect.setY(moveTo.x() + getWorldObjectBoundingRect().getPositionY());
    //}

    //auto items = scene()->items(rect, Qt::IntersectsItemShape);
    //for (auto pItem : items)
    //{

    //}
    return moveTo;
}

QVariant MapViewItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch (change)
    {
    //case ItemPositionChange:
    //    return QVariant(_checkMove(diff));

    case ItemPositionHasChanged:
        auto pos = Int32Point(value.toPoint().x(), value.toPoint().y());
        m_WorldObjectInfo.setPosition(pos);
        setZValue(m_BoundingRect.getBottom() + pos.getY());
        return value;
    }

    return QGraphicsPixmapItem::itemChange(change, value);
}

void MapViewItem::keyPressEvent(QKeyEvent* pEvent)
{
    //switch (pEvent->key())
    //{
    //case Qt::Key_Delete:
    //    if (auto* pScene = dynamic_cast<MapViewScene*>(scene()))
    //    {
    //        pScene->removeItem(this);
    //        pScene->getMapData().getWorldObjectInfoData().removeWorldObject(m_WorldObjectInfo.getGUID());
    //    }
    //    new DelayedDeleteObject<MapViewItem>(this);
    //    break;

    //case Qt::Key_Up:
    //    moveBy(0, -1);
    //    break;
    //case Qt::Key_Left:
    //    moveBy(-1, 0);
    //    break;
    //case Qt::Key_Down:
    //    moveBy(0, 1);
    //    break;
    //case Qt::Key_Right:
    //    moveBy(1, 0);
    //    break;
    //default: QGraphicsPixmapItem::keyPressEvent(pEvent);
    //}
}

Int32Point MapViewItem::getTopLeftPos() const
{
    return Int32Point(x(), y());
}

void MapViewItem::setTopLeftPos(Int32Point pos)
{
    setPos(pos.getX(), pos.getY());
}

void MapViewItem::setTopLeftPos(int32 x, int32 y)
{
    setPos(x, y);
}

Int32Point MapViewItem::getBottomRightPos() const
{
    return Int32Point(pos().x() + pixmap().width(), pos().y() + pixmap().height());
}

void MapViewItem::setBottomRightPos(Int32Point pos)
{
    setBottomRightPos(pos.getX(), pos.getY());
}

void MapViewItem::setBottomRightPos(int32 x, int32 y)
{
    setPos(x - pixmap().width(), y - pixmap().height());
}

Int32Point MapViewItem::getCenterPos() const
{
    return Int32Point(pos().y() + pixmap().height()/2, pos().x() + pixmap().width()/2);
}

void MapViewItem::setCenterPos(Int32Point pos)
{
    setCenterPos(pos.getX(), pos.getY());
}

void MapViewItem::setCenterPos(int32 x, int32 y)
{
    setPos(x - pixmap().width() / 2, y - pixmap().height() / 2);
}
