#include "ScriptAreaItem.h"
#include "EditorGlobal.h"
#include <DelayedDeleteObject.h>
#include <ScriptArea_AreaRect.h>
#include <ScriptArea.h>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGlobal.h>

/*#####
# PointMoveItem
#####*/
PointMoveItem::PointMoveItem(uint32 index, QGraphicsItem* pItem) : QGraphicsItem(pItem), m_Index(index)
{
    setFlag(QGraphicsItem::ItemIgnoresParentOpacity);
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

/*#####
# ScriptAreaItem
#####*/
ScriptAreaItem::ScriptAreaItem() : QGraphicsItem()
{
}

void ScriptAreaItem::setup(MAP::SCRIPT_AREA::ScriptArea* pScriptArea)
{
    for (auto pChild : childItems())
    {
        scene()->removeItem(pChild);
        new DelayedDeleteObject<QGraphicsItem>(pChild);
    }

    prepareGeometryChange();
    m_pScriptArea = pScriptArea;
    if (!m_pScriptArea)
        return;
    auto rect = GEOMETRY::boundingRect(*m_pScriptArea->getArea());
    setPos(QPoint(rect.getLeft(), rect.getTop()));
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
            auto p = m_pScriptArea->getArea()->getPoint(i+1);
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
    auto rect = GEOMETRY::boundingRect(*m_pScriptArea->getArea());
    return QRectF(rect.getLeft() - x() - 2, rect.getTop() - y() - 2, rect.getWidth() + 4, rect.getHeight() + 4);
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

bool ScriptAreaItem::sceneEventFilter(QGraphicsItem* pItem, QEvent* pEvent)
{
    if (m_pScriptArea)
    {
        if (pEvent->type() == QEvent::GraphicsSceneMouseMove)
        {
            if (auto pMoveEvent = dynamic_cast<QGraphicsSceneMouseEvent*>(pEvent))
            {
                if (pMoveEvent->buttons() & Qt::LeftButton)
                {
                    if (auto pEdgeItem = dynamic_cast<PointMoveItem*>(pItem))
                    {
                        prepareGeometryChange();
                        auto edge = m_pScriptArea->getArea()->getPoint(pEdgeItem->getIndex());
                        auto diff = decltype(edge)(pMoveEvent->scenePos().x() - edge.getX(), pMoveEvent->scenePos().y() - edge.getY());
                        edge += diff;
                        m_pScriptArea->getArea()->setPoint(pEdgeItem->getIndex(), edge);
                        _updateEdgePositions();
                        return true;
                    }
                }
            }
        }
    }
    return QGraphicsItem::sceneEventFilter(pItem, pEvent);
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
