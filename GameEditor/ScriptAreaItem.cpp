#include "ScriptAreaItem.h"
#include "moc_ScriptAreaItem.h"
#include "EditorGlobal.h"
#include <DelayedDeleteObject.h>
#include <Map/ScriptArea/AreaRect.h>
#include <Map/ScriptArea/ScriptArea.h>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGlobal.h>

/*#####
# PointMoveItem
#####*/
PointMoveItem::PointMoveItem(uint32 index, QGraphicsItem* pItem)
    : QGraphicsObject(pItem), m_Index(index)
{
    setFlag(QGraphicsItem::ItemIgnoresParentOpacity);

    QPen newPen;
    newPen.setWidth(1);
    newPen.setColor(Qt::black);
    setPen(newPen);
}

void PointMoveItem::setPen(const QPen& pen)
{
    prepareGeometryChange();
    m_Pen = pen;
    update();
}

QRectF PointMoveItem::boundingRect() const
{
    QSize size(2 * 3 + m_Pen.width(), 2 * 3 + m_Pen.width());
    return QRectF(0 - size.width() / 2, 0 - size.height() / 2, size.width(), size.height());
}

void PointMoveItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    pPainter->setPen(m_Pen);
    pPainter->drawPoint(0, 0);

    if (pOption->state & QStyle::State_Selected)
        highlightSelection(*this, pPainter, pOption);
}

int PointMoveItem::type() const
{
    return MAPPING_MODE::ITEM_MOVE_POINT;
}

void PointMoveItem::_posModifyStart(int key)
{
    m_Pos.insert(std::make_pair(key, scenePos().toPoint()));
}

void PointMoveItem::_posModifyEnd(int key)
{
    auto itr = m_Pos.find(key);
    if (itr == m_Pos.end())
        return;

    if (itr->second != scenePos().toPoint())
        emit positionChanged(m_Index, itr->second);
    m_Pos.erase(itr);
}

void PointMoveItem::keyPressEvent(QKeyEvent *pEvent)
{
    const int32 speed = 1;
    switch (pEvent->key())
    {
    case Qt::Key_Down:
        if (!pEvent->isAutoRepeat())
            _posModifyStart(pEvent->key());
        moveBy(0, speed);
        emit positionChanged(m_Index);
        break;

    case Qt::Key_Up:
        if (!pEvent->isAutoRepeat())
            _posModifyStart(pEvent->key());
        moveBy(0, -speed);
        emit positionChanged(m_Index);
        break;

    case Qt::Key_Left:
        if (!pEvent->isAutoRepeat())
            _posModifyStart(pEvent->key());
        moveBy(-speed, 0);
        emit positionChanged(m_Index);
        break;

    case Qt::Key_Right:
        if (!pEvent->isAutoRepeat())
            _posModifyStart(pEvent->key());
        moveBy(speed, 0);
        emit positionChanged(m_Index);
        break;

    default:
        QGraphicsObject::keyPressEvent(pEvent);
    }
}

void PointMoveItem::keyReleaseEvent(QKeyEvent *pEvent)
{
    switch (pEvent->key())
    {
    case Qt::Key_Down:
    case Qt::Key_Up:
    case Qt::Key_Left:
    case Qt::Key_Right:
        if (!pEvent->isAutoRepeat())
            _posModifyEnd(pEvent->key());
        break;
    default:
        QGraphicsObject::keyReleaseEvent(pEvent);
    }
}

void PointMoveItem::mousePressEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsObject::mousePressEvent(pEvent);
    if (pEvent->button() == Qt::LeftButton)
        _posModifyStart(pEvent->button());
}

void PointMoveItem::mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsObject::mouseMoveEvent(pEvent);
    auto itr = m_Pos.find(Qt::LeftButton);
    if (itr != m_Pos.end())
        emit positionChanged(m_Index);
}

void PointMoveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent)
{
    QGraphicsObject::mouseReleaseEvent(pEvent);
    if (pEvent->button() == Qt::LeftButton)
        _posModifyEnd(pEvent->button());
}

/*#####
# ScriptAreaItem
#####*/
ScriptAreaItem::ScriptAreaItem()
    : QGraphicsObject()
{}

QVariant ScriptAreaItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch (change)
    {
    case QGraphicsItem::ItemPositionChange:
        m_pScriptArea->getArea()->moveBy(value.toPoint().x() - x(), value.toPoint().y() - y());
        return value;
        break;
    }
    return QGraphicsObject::itemChange(change, value);
}

void ScriptAreaItem::setup(MAP::SCRIPT_AREA::ScriptArea* pScriptArea)
{
    for (auto pChild : childItems())
    {
        scene()->removeItem(pChild);
        new DelayedDeleteObject<QGraphicsItem>(pChild);
    }

    prepareGeometryChange();
    m_pScriptArea = nullptr;
    if (!m_pScriptArea || m_pScriptArea->isEmpty())
        return;

    m_pScriptArea = pScriptArea;
    auto rect = m_pScriptArea->getArea()->getBoundingRect();
    setPos(QPoint(rect.getX(), rect.getY()));
    for (uint32 i = 0; i < m_pScriptArea->getArea()->pointCount(); ++i)
        _setupPointMoveItem(i);
}

int ScriptAreaItem::type() const
{
    return MAPPING_MODE::ITEM_SCRIPT_AREA;
}

void ScriptAreaItem::_setupPointMoveItem(uint32 index)
{
    if (!m_pScriptArea)
        return;
    auto pItem = new PointMoveItem(index, this);
    pItem->installSceneEventFilter(this);
    auto pos = m_pScriptArea->getArea()->getPoint(index);
    pItem->setPos(pos.getX() - x(), pos.getY() - y());
    QPen newPen;
    newPen.setWidth(5);
    newPen.setColor(Qt::yellow);
    pItem->setPen(newPen);
    connect(pItem, SIGNAL(positionChanged(uint32, QPoint)), this, SLOT(_childPositionChanged(uint32, QPoint)));
    connect(pItem, SIGNAL(positionChanged(uint32)), this, SLOT(_childPositionChanged(uint32)));
}

void ScriptAreaItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    if (!m_pScriptArea)
        return;
    // setup pen
    QPen newPen;
    newPen.setWidth(3);
    newPen.setColor(Qt::yellow);
    newPen.setStyle(Qt::DashLine);
    pPainter->setPen(newPen);

    // paint
    auto count = m_pScriptArea->getArea()->pointCount();
    for (uint32 i = 0; i < count; ++i)
    {
        QPoint endPoint;
        if (i + 1 == count)
        {
            auto p = m_pScriptArea->getArea()->getPoint(0);
            endPoint.setX(p.getX() - x());
            endPoint.setY(p.getY() - y());
        }
        else
        {
            auto p = m_pScriptArea->getArea()->getPoint(i + 1);
            endPoint.setX(p.getX() - x());
            endPoint.setY(p.getY() - y());
        }

        auto p = m_pScriptArea->getArea()->getPoint(i);
        pPainter->drawLine(QPoint(p.getX() - x(), p.getY() - y()), endPoint);
    }

    if (pOption->state & QStyle::State_Selected)
        highlightSelection(*this, pPainter, pOption);
}

QRectF ScriptAreaItem::boundingRect() const
{
    if (!m_pScriptArea)
        return QRectF();
    auto rect = m_pScriptArea->getArea()->getBoundingRect();
    return QRectF(rect.getX() - x() - 2, rect.getY() - y() - 2, rect.getWidth() + 4, rect.getHeight() + 4);
}

void ScriptAreaItem::setEditable(bool editable /* = true */)
{
    setFlag(ItemIsSelectable, editable);
    setFlag(ItemIsFocusable, editable);
    setVisible(editable);

    for (auto pItem : childItems())
    {
        pItem->setFlag(ItemIsMovable, editable);
        pItem->setFlag(ItemIsSelectable, editable);
        pItem->setFlag(ItemIsFocusable, editable);
        pItem->setVisible(editable);
    }
}

const MAP::SCRIPT_AREA::ScriptArea* ScriptAreaItem::getScriptArea() const
{
    return m_pScriptArea;
}

MAP::SCRIPT_AREA::ScriptArea* ScriptAreaItem::getScriptArea()
{
    return m_pScriptArea;
}

PointMoveItem* ScriptAreaItem::getPointItem(uint32 index) const
{
    for (auto pItem : childItems())
    {
        if (auto pPItem = dynamic_cast<PointMoveItem*>(pItem))
        {
            if (pPItem->getIndex() == index)
                return pPItem;
        }
    }
    return nullptr;
}

void ScriptAreaItem::_updateEdgePositions()
{
    if (!m_pScriptArea)
        return;

    for (auto pItem : childItems())
    {
        if (auto pEdgeItem = dynamic_cast<PointMoveItem*>(pItem))
        {
            auto pos = m_pScriptArea->getArea()->getPoint(pEdgeItem->getIndex());
            pEdgeItem->setPos(pos.getX() - x(), pos.getY() - y());
        }
    }
}

void ScriptAreaItem::_childPositionChanged(uint32 index, QPoint pos)
{
    if (!m_pScriptArea)
        return;

    emit modified(m_pScriptArea->getGUID(), index, pos);
}

void ScriptAreaItem::_childPositionChanged(uint32 index)
{
    if (auto pEdgeItem = getPointItem(index))
    {
        prepareGeometryChange();
        auto edge = m_pScriptArea->getArea()->getPoint(pEdgeItem->getIndex());
        auto diff = decltype(edge)(pEdgeItem->scenePos().x() - edge.getX(), pEdgeItem->scenePos().y() - edge.getY());
        edge += diff;
        m_pScriptArea->getArea()->setPoint(pEdgeItem->getIndex(), edge);
        _updateEdgePositions();
    }
}

void ScriptAreaItem::setPoint(uint32 index, const GEOMETRY::Point<int32>& pos)
{
    if (!m_pScriptArea || !m_pScriptArea->getArea() || m_pScriptArea->getArea()->pointCount() <= index)
        return;

    m_pScriptArea->getArea()->setPoint(index, pos);
    _updateEdgePositions();
}
