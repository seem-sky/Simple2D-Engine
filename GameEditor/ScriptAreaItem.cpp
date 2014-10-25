#include "ScriptAreaItem.h"
#include <DelayedDeleteObject.h>
#include <ScriptArea_AreaRect.h>
#include <ScriptArea.h>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>

static void highlightSelection(const QGraphicsItem& item, QPainter* pPainter, const QStyleOptionGraphicsItem* pOption)
{
    // from QGraphicsItem.cpp
    const QRectF murect = pPainter->transform().mapRect(QRectF(0, 0, 1, 1));
    if (qFuzzyCompare(qMax(murect.width(), murect.height()) + 1, 1))
        return;

    const QRectF mbrect = pPainter->transform().mapRect(item.boundingRect());
    if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
        return;

    qreal itemPenWidth = 1.0;
    const qreal pad = itemPenWidth / 2;

    const qreal penWidth = 0; // cosmetic pen

    const QColor fgcolor = pOption->palette.windowText().color();
    const QColor bgcolor( // ensure good contrast against fgcolor
        fgcolor.red() > 127 ? 0 : 255,
        fgcolor.green() > 127 ? 0 : 255,
        fgcolor.blue() > 127 ? 0 : 255);

    pPainter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
    pPainter->setBrush(Qt::NoBrush);
    pPainter->drawRect(item.boundingRect().adjusted(pad, pad, -pad, -pad));

    pPainter->setPen(QPen(pOption->palette.windowText(), 0, Qt::DashLine));
    pPainter->setBrush(Qt::NoBrush);
}

/*#####
# PointMoveItem
#####*/
PointMoveItem::PointMoveItem(uint32 index, QGraphicsItem* pItem) : QGraphicsItem(pItem), m_Index(index)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
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
    return UserType + 2;
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
    return UserType + 3;
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
    //setFlag(ItemIsMovable, editable);
    //setFlag(ItemIsSelectable, editable);
    setVisible(editable);
}

const MAP::SCRIPT_AREA::ScriptArea* ScriptAreaItem::getScriptArea() const
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
