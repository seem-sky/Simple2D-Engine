#include "AnimationEditorItem.h"
#include "moc_AnimationEditorItem.h"
#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QGraphicsSceneWheelEvent>
#include <math_extensions.h>
#include <QtGlobal.h>
#include <QtWidgets/QGraphicsScene>

using namespace animation;

/*#####
# EditorItem
#####*/
EditorItem::EditorItem()
    : QGraphicsObject()
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

void EditorItem::setModifyFlag(ModifyFlags flag, bool apply)
{
    m_Flags.setFlag(flag, apply);
}

void EditorItem::_setup(const ItemData& data)
{
    setX(data.getX());
    setY(data.getY());
    setZValue(data.getZValue());
    setRotation(data.getRotation());
    setScale(data.getScale());
    setOpacity(data.getOpacity());
}

void EditorItem::_setupFrom(ItemData& data) const
{
    data.setX(x());
    data.setY(y());
    data.setZValue(zValue());
    data.setScale(scale());
    data.setRotation(rotation());
    data.setOpacity(opacity());
}

QVariant EditorItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    switch (change)
    {
    case QGraphicsItem::ItemOpacityChange:
        if (!m_Flags.hasFlag(EditorItem::ModifyFlags::opacity))
            return QVariant(opacity());
        return QVariant(MATH::between(value.toFloat(),animation::SpriteData::minimumOpacity(), animation::SpriteData::maximumOpacity()));

    case QGraphicsItem::ItemScaleChange:
        if (!m_Flags.hasFlag(EditorItem::ModifyFlags::scalable))
            return QVariant(scale());
        return QVariant(MATH::between(value.toFloat(), animation::SpriteData::minimumScale(), animation::SpriteData::maximumScale()));

    case QGraphicsItem::ItemZValueChange:
        if (!m_Flags.hasFlag(EditorItem::ModifyFlags::zValue))
            return QVariant(zValue());
        return QVariant(MATH::between(value.toFloat(), animation::SpriteData::minimumZValue(), animation::SpriteData::maximumZValue()));

    case QGraphicsItem::ItemRotationChange:
    {
        if (!m_Flags.hasFlag(EditorItem::ModifyFlags::rotating))
            return QVariant(rotation());

        const int32 mod = 1000;
        int32 rot = value.toDouble() * mod;
        if (rot < 0)
        {
            rot %= -360 * mod;
            rot += 360 * mod;
        }
        else
            rot %= 360 * mod;
        return QVariant(rot / mod);
    }

    case QGraphicsItem::ItemZValueHasChanged:
    case QGraphicsItem::ItemOpacityHasChanged:
    case QGraphicsItem::ItemScenePositionHasChanged:
    case QGraphicsItem::ItemRotationHasChanged:
    case QGraphicsItem::ItemScaleHasChanged:
        _emitChange();
        return value;
    }
    return QGraphicsObject::itemChange(change, value);
}

void EditorItem::keyPressEvent(QKeyEvent* pEvent)
{
    qreal speed = 1;
    if (pEvent->modifiers() & Qt::ShiftModifier)
        speed = 10;
    switch (pEvent->key())
    {
    case Qt::Key_Escape: setSelected(false); break;
    case Qt::Key_Delete:
        if (scene())
            scene()->removeItem(this);
        deleteLater();
        break;

        // position
    case Qt::Key_Down: moveBy(0, speed); break;
    case Qt::Key_Up: moveBy(0, -speed); break;
    case Qt::Key_Left: moveBy(-speed, 0); break;
    case Qt::Key_Right: moveBy(speed, 0); break;

        // z value
    case Qt::Key_Plus: setScale(scale() + speed / 10); break;
    case Qt::Key_Minus: setScale(scale() - speed / 10); break;

    default:
        QGraphicsObject::keyPressEvent(pEvent);
    }
}

void EditorItem::wheelEvent(QGraphicsSceneWheelEvent* pEvent)
{
    int32 mod = pEvent->delta() / 15;
    if (pEvent->modifiers())
        mod = 90;

    setRotation(rotation() + (pEvent->delta() ? mod : -mod));
}

/*#####
# SpriteEditorItem
#####*/
SpriteEditorItem::SpriteEditorItem(const helper::CreatorInterface& creator)
    : EditorItem(), m_Creator(creator)
{
    setModifyFlag(ModifyFlags::opacity, true);
    setModifyFlag(ModifyFlags::rotating, true);
    setModifyFlag(ModifyFlags::scalable, true);
    setModifyFlag(ModifyFlags::zValue, true);
}

void SpriteEditorItem::_emitChange()
{
    emit changed(this);
}

SpriteEditorItem::SpriteEditorItem(const animation::SpriteData& sprite, const helper::CreatorInterface& creator)
    : SpriteEditorItem(creator)
{
    setup(sprite);
}

void SpriteEditorItem::setID(uint32 ID)
{
    m_ID = ID;
    m_Pixmap = m_Creator.createPixmap(ID);
    setTransformOriginPoint(m_Pixmap.rect().center());
    prepareGeometryChange();
    emit changed(this);
}

uint32 SpriteEditorItem::getID() const
{
    return m_ID;
}

void SpriteEditorItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    pPainter->drawPixmap(0, 0, m_Pixmap);

    if (pOption->state & QStyle::State_Selected)
        highlightSelection(*this, pPainter, pOption);
}

QRectF SpriteEditorItem::boundingRect() const
{
    return m_Pixmap.rect();
}

animation::SpriteData SpriteEditorItem::createSpriteData() const
{
    animation::SpriteData sprite(getID());
    _setupFrom(sprite);
    return sprite;
}

void SpriteEditorItem::setup(const animation::SpriteData& sprite)
{
    if (sprite.isEmpty())
        throw std::runtime_error("Unable to setup from empty sprite.");
    
    _setup(sprite);
    setID(sprite.getID());
}

/*#####
# AdditionEditorItem
#####*/
AdditionEditorItem::AdditionEditorItem()
    : EditorItem()
{}

void AdditionEditorItem::_emitChange()
{
    emit changed(this);
}

uint32 AdditionEditorItem::getAdditionTypeID() const
{
    return m_AdditionTypeID;
}

void AdditionEditorItem::setAdditionTypeID(uint32 ID)
{
    m_AdditionTypeID = ID;
}

void AdditionEditorItem::_setup(const AdditionData& data)
{
    m_AdditionTypeID = data.getAdditionTypeID();
    EditorItem::_setup(data);
}

void AdditionEditorItem::_setupFrom(AdditionData& data) const
{
    data.setAdditionTypeID(getAdditionTypeID());
    EditorItem::_setupFrom(data);
}

/*#####
# PointAdditionEditorItem
#####*/
PointAdditionEditorItem::PointAdditionEditorItem()
    : AdditionEditorItem()
{
    setFlag(QGraphicsItem::ItemIgnoresParentOpacity);

    QPen newPen;
    newPen.setWidth(1);
    newPen.setColor(Qt::red);
    setPen(newPen);
    setCrossSize(10, 10);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

PointAdditionEditorItem::PointAdditionEditorItem(const AdditionData& sprite)
    : PointAdditionEditorItem()
{
    setup(sprite);
}

AdditionData PointAdditionEditorItem::createAdditionData() const
{
    animation::AdditionData data;
    data.setType(AdditionData::Type::point);
    _setupFrom(data);
    return data;
}

void PointAdditionEditorItem::setup(const AdditionData& data)
{
    if (data.getType() != AdditionData::Type::point)
        throw std::runtime_error("Invalid type.");

    _setup(data);
}

QSize PointAdditionEditorItem::getCrossSize() const
{
    return m_Cross.getCrossSize();
}

void PointAdditionEditorItem::setCrossSize(const QSize& size)
{
    m_Cross.setCrossSize(size);
    prepareGeometryChange();
}

void PointAdditionEditorItem::setCrossSize(int width, int height)
{
    setCrossSize(QSize(width, height));
}

const QPen& PointAdditionEditorItem::getPen() const
{
    return m_Cross.getPen();
}

void PointAdditionEditorItem::setPen(const QPen& pen)
{
    m_Cross.setPen(pen);
    update();
    prepareGeometryChange();
}

QRectF PointAdditionEditorItem::boundingRect() const
{
    return m_Cross.boundingRect();
}

void PointAdditionEditorItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    pPainter->setPen(m_Cross.getPen());
    m_Cross.paint(pPainter);

    if (pOption->state & QStyle::State_Selected)
        highlightSelection(*this, pPainter, pOption);
}
