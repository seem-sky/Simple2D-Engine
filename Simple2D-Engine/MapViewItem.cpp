#include "MapViewItem.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtGui/QPainter>

MapViewItem::MapViewItem(MAP::MAP_DATA::WorldObjectInfo& info, const QPixmap& pixmap) : QGraphicsPixmapItem(pixmap), m_WorldObjectInfo(info)
{
    setFlag(ItemSendsScenePositionChanges);
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);

    m_Flags.setFlag(Flags::DRAW_BOUNDING_RECT);
}

void MapViewItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /* = 0 */)
{
    QGraphicsPixmapItem::paint(pPainter, pOption, pWidget);
    if (m_Flags.hasFlag(Flags::DRAW_BOUNDING_RECT))
    {
        pPainter->setPen(Qt::red);
        pPainter->drawRect(m_BoundingRect.getLeft(), m_BoundingRect.getTop(), m_BoundingRect.getWidth(), m_BoundingRect.getHeight());
    }
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
        auto pos = GEOMETRY::Point<int32>(value.toPoint().x(), value.toPoint().y());
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

GEOMETRY::Point<int32> MapViewItem::getTopLeftPos() const
{
    return GEOMETRY::Point<int32>(x(), y());
}

void MapViewItem::setTopLeftPos(GEOMETRY::Point<int32> pos)
{
    setPos(pos.getX(), pos.getY());
}

void MapViewItem::setTopLeftPos(int32 x, int32 y)
{
    setPos(x, y);
}

GEOMETRY::Point<int32> MapViewItem::getBottomRightPos() const
{
    return GEOMETRY::Point<int32>(pos().x() + pixmap().width(), pos().y() + pixmap().height());
}

void MapViewItem::setBottomRightPos(GEOMETRY::Point<int32> pos)
{
    setBottomRightPos(pos.getX(), pos.getY());
}

void MapViewItem::setBottomRightPos(int32 x, int32 y)
{
    setPos(x - pixmap().width(), y - pixmap().height());
}

GEOMETRY::Point<int32> MapViewItem::getCenterPos() const
{
    return GEOMETRY::Point<int32>(pos().y() + pixmap().height()/2, pos().x() + pixmap().width()/2);
}

void MapViewItem::setCenterPos(GEOMETRY::Point<int32> pos)
{
    setCenterPos(pos.getX(), pos.getY());
}

void MapViewItem::setCenterPos(int32 x, int32 y)
{
    setPos(x - pixmap().width() / 2, y - pixmap().height() / 2);
}
