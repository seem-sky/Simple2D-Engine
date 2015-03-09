#include "AnimationAdditionEditorScene.h"
#include "moc_AnimationAdditionEditorScene.h"
#include <Database/Prototype/Derived.h>

using namespace animation::addition;

/*#####
# EditorScene
#####*/
// methods
EditorScene::EditorScene(const helper::CreatorInterface& creator, QObject* pParent)
    : animation::EditorScene(creator, pParent)
{}

void EditorScene::setup(const database::prototype::AnimationAddition* pAddition)
{
    for (uint32 i = 0; i < pAddition->getSize(); ++i)
        addSprite(pAddition->getSprite(i));
}

void EditorScene::setupFrom(database::prototype::AnimationAddition* pAddition) const
{
    for (auto pItem : items())
    {
        if (auto pAddItem = dynamic_cast<SpriteEditorItem*>(pItem))
            pAddition->addSprite(pAddItem->createSpriteData());
    }
}

/*#####
# PointEditorScene
#####*/
PointEditorScene::PointEditorScene(const helper::CreatorInterface& creator, QObject* pParent)
    : EditorScene(creator, pParent), m_pPointItem(new PointAdditionEditorItem())
{
    addItem(m_pPointItem);
    m_pPointItem->setZValue(animation::SpriteData::maximumZValue() + 1);

    QPen newPen;
    newPen.setWidth(1);
    newPen.setColor(Qt::red);
    m_pPointItem->setPen(newPen);
    m_pPointItem->setCrossSize(10, 10);
    m_pPointItem->setFlag(QGraphicsItem::ItemIsMovable);
    m_pPointItem->setFlag(QGraphicsItem::ItemIsSelectable);
}

void PointEditorScene::clearContents()
{
    removeItem(m_pPointItem);
    clear();
    addItem(m_pPointItem);
    m_pPointItem->setPos(0, 0);
}

void PointEditorScene::setup(const database::prototype::AnimationAddition* pAddition)
{
    EditorScene::setup(pAddition);

    // ugly hack, but I think this is ok, because I have no pleasure to think about an other solution
    if (auto pObj = dynamic_cast<const database::prototype::AnimationAdditionPoint*>(pAddition))
    {
        m_pPointItem->setX(pObj->getX());
        m_pPointItem->setY(pObj->getY());
    }
}

void PointEditorScene::setupFrom(database::prototype::AnimationAddition* pAddition) const
{
    EditorScene::setupFrom(pAddition);

    // ugly hack, but I think this is ok, because I have no pleasure to think about an other solution
    if (auto pObj = dynamic_cast<database::prototype::AnimationAdditionPoint*>(pAddition))
    {
        pObj->setX(m_pPointItem->x());
        pObj->setY(m_pPointItem->y());
    }
}
