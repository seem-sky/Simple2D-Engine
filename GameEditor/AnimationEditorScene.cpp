#include "AnimationEditorScene.h"
#include "moc_AnimationEditorScene.h"
#include <QtWidgets/QGraphicsSceneDragDropEvent>
#include <QtGui/QPainter>
#include <QtCore/QMimeData>

using namespace animation;

// free functions
static const uint32 GRID_SPACE = 10;
static void drawGrid(QPainter* pPainter, const QRectF& rect)
{
    // draw 0 lines
    QPainterPath painterPath;
    pPainter->setPen(QColor(255, 0, 0));
    if (rect.y() <= 0 && rect.y() + rect.height() >= 0)
    {
        painterPath.moveTo(rect.x(), 0);
        painterPath.lineTo(rect.x() + rect.width(), 0);
    }
    if (rect.x() <= 0 && rect.x() + rect.width() >= 0)
    {
        painterPath.moveTo(0, rect.y());
        painterPath.lineTo(0, rect.y() + rect.height());
    }
    pPainter->drawPath(painterPath);

    // horizontal lines
    pPainter->setPen(QColor(0, 0, 0));
    QPainterPath linePath;
    for (int i = int(rect.y() / GRID_SPACE)*GRID_SPACE; i < rect.y() + rect.height(); i += GRID_SPACE)
    {
        if (i == 0)
            continue;
        linePath.moveTo(rect.x(), i);
        linePath.lineTo(rect.x() + rect.width(), i);
    }

    // vertical lines
    for (int i = int(rect.x() / GRID_SPACE)*GRID_SPACE; i < rect.x() + rect.width(); i += GRID_SPACE)
    {
        if (i == 0)
            continue;
        linePath.moveTo(i, rect.y());
        linePath.lineTo(i, rect.y() + rect.height());
    }
    pPainter->drawPath(linePath);
}

static SpriteEditorItem* getSelectedSpriteItem(EditorScene* pScene)
{
    auto pItems = pScene->selectedItems();
    if (!pItems.empty())
        return dynamic_cast<SpriteEditorItem*>(pItems.first());
    return nullptr;
}

static AdditionEditorItem* getSelectedAdditionItem(EditorScene* pScene)
{
    auto pItems = pScene->selectedItems();
    if (!pItems.empty())
        return dynamic_cast<AdditionEditorItem*>(pItems.first());
    return nullptr;
}

static EditorItem* getSelectedItem(EditorScene* pScene)
{
    auto pItems = pScene->selectedItems();
    if (!pItems.empty())
        return dynamic_cast<EditorItem*>(pItems.first());
    return nullptr;
}

// methods
EditorScene::EditorScene(const helper::CreatorInterface& creator, QObject* pParent)
    : QGraphicsScene(pParent), m_Creator(creator)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(_onSelectionChanged()));
}

void EditorScene::dragEnterEvent(QGraphicsSceneDragDropEvent* pEvent)
{
    pEvent->acceptProposedAction();
}

void EditorScene::dragMoveEvent(QGraphicsSceneDragDropEvent* pEvent)
{
    pEvent->accept();
}

void EditorScene::dropEvent(QGraphicsSceneDragDropEvent* pEvent)
{
    uint32 ID = pEvent->mimeData()->text().toUInt();
    if (ID)
    {
        clearSelection();
        SpriteData sprite(ID);
        sprite.setX(pEvent->scenePos().x());
        sprite.setY(pEvent->scenePos().y());
        auto pItem = addSprite(sprite);
        pItem->setSelected(true);
    }
}

void EditorScene::drawBackground(QPainter* pPainter, const QRectF& rect)
{
    drawGrid(pPainter, rect);
}

AdditionEditorItem* EditorScene::addAddition(const animation::AdditionData& data)
{
    AdditionEditorItem* pItem = nullptr;
    switch (data.getType())
    {
        case AdditionData::Type::point: pItem = new PointAdditionEditorItem(); break;
        case AdditionData::Type::rect: break;
    }

    if (pItem)
    {
        pItem->setModifyFlag(EditorItem::ModifyFlags::rotating, true);
        pItem->setModifyFlag(EditorItem::ModifyFlags::scalable, true);
        pItem->setModifyFlag(EditorItem::ModifyFlags::zValue, true);
        addItem(pItem);
        connect(pItem, SIGNAL(changed(const AdditionEditorItem*)), this, SLOT(_onSpriteItemChanged(const AdditionEditorItem*)));
    }
    return pItem;
}

SpriteEditorItem* EditorScene::addSprite(const animation::SpriteData& data)
{
    auto pItem = new SpriteEditorItem(data, m_Creator);
    addItem(pItem);
    connect(pItem, SIGNAL(changed(const SpriteEditorItem*)), this, SLOT(_onSpriteItemChanged(const SpriteEditorItem*)));
    return pItem;
}

FrameData EditorScene::getFrame() const
{
    FrameData frame;
    for (auto pItem : items())
    {
        if (auto pSprite = dynamic_cast<SpriteEditorItem*>(pItem))
            frame.addSprite(pSprite->createSpriteData());
        else if (auto pAddition = dynamic_cast<AdditionEditorItem*>(pItem))
            frame.addAddition(pAddition->createAdditionData());
    }
    return frame;
}

void EditorScene::setup(const FrameData& frame)
{
    clear();
    for (uint32 i = 0; i < frame.getSpriteCount(); ++i)
        addSprite(frame.getSprite(i));
    for (uint32 i = 0; i < frame.getAdditionCount(); ++i)
        addAddition(frame.getAddition(i));
}

// slots
void EditorScene::_onSelectionChanged()
{
    emit selectionCleared();
    if (auto pItem = getSelectedSpriteItem(this))
        emit selectionDataChanged(pItem->createSpriteData());
    else if (auto pItem = getSelectedAdditionItem(this))
        emit selectionDataChanged(pItem->createAdditionData());
}

void EditorScene::_onSpriteItemChanged(const animation::SpriteEditorItem* pItem)
{
    if (pItem && pItem->isSelected())
        emit selectionDataChanged(pItem->createSpriteData());
}

void EditorScene::_onSpriteItemChanged(const animation::AdditionEditorItem* pItem)
{
    if (pItem && pItem->isSelected())
        emit selectionDataChanged(pItem->createAdditionData());
}

void EditorScene::onSizeChanged(const QSize& size)
{

}

void EditorScene::onPositionChanged(const QPoint& pos)
{
    if (auto pItem = getSelectedItem(this))
        pItem->setPos(pos);
}

void EditorScene::onZValueChanged(qreal zValue)
{
    if (auto pItem = getSelectedItem(this))
        pItem->setZValue(zValue);
}

void EditorScene::onScaleChanged(qreal scale)
{
    if (auto pItem = getSelectedItem(this))
        pItem->setScale(scale);
}

void EditorScene::onRotationChanged(qreal rotation)
{
    if (auto pItem = getSelectedItem(this))
        pItem->setRotation(rotation);
}

void EditorScene::onOpacityChanged(qreal opacity)
{
    if (auto pItem = getSelectedItem(this))
        pItem->setOpacity(opacity);
}

void EditorScene::onAdditionTypeIDChanged(uint32 ID)
{
    if (auto pItem = getSelectedAdditionItem(this))
        pItem->setAdditionTypeID(ID);
}

void EditorScene::onPointAdditionAdd()
{
    AdditionData data;
    data.setType(AdditionData::Type::point);
    addAddition(data);
}

void EditorScene::onRectAdditionAdd()
{
    AdditionData data;
    data.setType(AdditionData::Type::rect);
    addAddition(data);
}
