#include "WorldObjectItem.h"
#include "EditorGlobal.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtGui/QPainter>

WorldObjectItem::WorldObjectItem(MAP::MAP_DATA::WorldObjectInfo& info) : QGraphicsPixmapItem(), QObject(), m_WorldObjectInfo(info)
{
    setFlag(ItemSendsScenePositionChanges);
    m_Flags.setFlag(Flags::DRAW_BOUNDING_RECT);
}

void WorldObjectItem::setEditable(bool editable)
{
    setFlag(ItemIsMovable, editable);
    setFlag(ItemIsSelectable, editable);
}

void WorldObjectItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /* = 0 */)
{
    QGraphicsPixmapItem::paint(pPainter, pOption, pWidget);
    if (m_Flags.hasFlag(Flags::DRAW_BOUNDING_RECT))
    {
        pPainter->setPen(Qt::red);
        pPainter->drawRect(m_BoundingRect.getLeft(), m_BoundingRect.getTop(), m_BoundingRect.getWidth(), m_BoundingRect.getHeight());
    }
}

QPoint WorldObjectItem::_checkMove(QPoint moveTo) const
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

QVariant WorldObjectItem::itemChange(GraphicsItemChange change, const QVariant& value)
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

GEOMETRY::Point<int32> WorldObjectItem::getTopLeftPos() const
{
    return GEOMETRY::Point<int32>(x(), y());
}

void WorldObjectItem::setTopLeftPos(GEOMETRY::Point<int32> pos)
{
    setPos(pos.getX(), pos.getY());
}

void WorldObjectItem::setTopLeftPos(int32 x, int32 y)
{
    setPos(x, y);
}

GEOMETRY::Point<int32> WorldObjectItem::getBottomRightPos() const
{
    return GEOMETRY::Point<int32>(pos().x() + pixmap().width(), pos().y() + pixmap().height());
}

void WorldObjectItem::setBottomRightPos(GEOMETRY::Point<int32> pos)
{
    setBottomRightPos(pos.getX(), pos.getY());
}

void WorldObjectItem::setBottomRightPos(int32 x, int32 y)
{
    setPos(x - pixmap().width(), y - pixmap().height());
}

GEOMETRY::Point<int32> WorldObjectItem::getCenterPos() const
{
    return GEOMETRY::Point<int32>(pos().y() + pixmap().height()/2, pos().x() + pixmap().width()/2);
}

void WorldObjectItem::setCenterPos(GEOMETRY::Point<int32> pos)
{
    setCenterPos(pos.getX(), pos.getY());
}

void WorldObjectItem::setCenterPos(int32 x, int32 y)
{
    setPos(x - pixmap().width() / 2, y - pixmap().height() / 2);
}

int WorldObjectItem::type() const
{
    return MAPPING_MODE::ITEM_WORLD_OBJECT;
}
