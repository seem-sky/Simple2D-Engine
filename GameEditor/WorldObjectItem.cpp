#include "WorldObjectItem.h"
#include "EditorGlobal.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGlobal.h>
#include "VisualViewer.h"

WorldObjectItem::WorldObjectItem(MAP::MAP_DATA::WorldObjectInfo& info, const DATABASE::DatabaseMgr& DBMgr)
    : QGraphicsItem(), QObject(), m_WorldObjectInfo(info), m_DBMgr(DBMgr)
{
    setFlag(ItemSendsScenePositionChanges);
    m_Flags.setFlag(Flags::DRAW_BOUNDING_RECT);
    _setup();
}

QRectF WorldObjectItem::boundingRect() const
{
    return QRectF(0, 0, m_Pixmap.width(), m_Pixmap.height());
}

void WorldObjectItem::_setup()
{
    VisualViewer viewer;
    viewer.setFrameShape(QFrame::NoFrame);
    viewer.showGrid(false);
    viewer.setDatabaseMgr(&m_DBMgr);
    auto pWorldObject = m_DBMgr.getWorldObjectDatabase()->getOriginalPrototype(m_WorldObjectInfo.getID());
    if (!pWorldObject)
        throw std::runtime_error("Invalid WorldObject ID.");

    auto &animationModule = pWorldObject->getAnimationModule();
    for (uint32 i = 0; i < animationModule.getAnimationCount(); ++i)
    {
        auto animationInfo = animationModule.getAnimationInfo(i);
        if (animationInfo.m_AnimationTypeID - 1 == static_cast<uint32>(m_WorldObjectInfo.getDirection()))
        {
            viewer.setAnimation(animationInfo);
            break;
        }
    }

    viewer.stopAnimation();
    viewer.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewer.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // set transparent background
    viewer.setStyleSheet("background:transparent;");
    viewer.setAttribute(Qt::WA_TranslucentBackground);
    viewer.setWindowFlags(Qt::FramelessWindowHint);

    auto rect = viewer.scene()->itemsBoundingRect();
    viewer.resizeToContent();
    prepareGeometryChange();
    m_Pixmap = viewer.grab();
}

void WorldObjectItem::setEditable(bool editable)
{
    setFlag(ItemIsMovable, editable);
    setFlag(ItemIsSelectable, editable);
}

void WorldObjectItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget /* = 0 */)
{
    pPainter->drawPixmap(0, 0, m_Pixmap);
    if (m_Flags.hasFlag(Flags::DRAW_BOUNDING_RECT))
    {
        pPainter->setPen(Qt::red);
        pPainter->drawRect(m_BoundingRect.getX(), m_BoundingRect.getY(), m_BoundingRect.getWidth(), m_BoundingRect.getHeight());
    }

    if (pOption->state & QStyle::State_Selected)
        highlightSelection(*this, pPainter, pOption);
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
        setZValue(m_BoundingRect.getY() + m_BoundingRect.getHeight() + pos.getY());
        return value;
    }

    return QGraphicsItem::itemChange(change, value);
}

void WorldObjectItem::setWorldObjectInfo(const MAP::MAP_DATA::WorldObjectInfo& info)
{
    m_WorldObjectInfo = info;

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
    return GEOMETRY::Point<int32>(pos().x() + m_Pixmap.width(), pos().y() + m_Pixmap.height());
}

void WorldObjectItem::setBottomRightPos(GEOMETRY::Point<int32> pos)
{
    setBottomRightPos(pos.getX(), pos.getY());
}

void WorldObjectItem::setBottomRightPos(int32 x, int32 y)
{
    setPos(x - m_Pixmap.width(), y - m_Pixmap.height());
}

GEOMETRY::Point<int32> WorldObjectItem::getCenterPos() const
{
    return GEOMETRY::Point<int32>(pos().x() + m_Pixmap.width() / 2, pos().y() + m_Pixmap.height() / 2);
}

void WorldObjectItem::setCenterPos(GEOMETRY::Point<int32> pos)
{
    setCenterPos(pos.getX(), pos.getY());
}

void WorldObjectItem::setCenterPos(int32 x, int32 y)
{
    setPos(x - m_Pixmap.width() / 2, y - m_Pixmap.height() / 2);
}

int WorldObjectItem::type() const
{
    return MAPPING_MODE::ITEM_WORLD_OBJECT;
}
