#include "DatabaseModuleAnimation.h"
#include "moc_DatabaseModuleAnimation.h"
#include "DelayedDeleteObject.h"
#include <QtWidgets/QAction>

// ToDo: Final coding of revert actions (many improvements needed)

/*#####
# AnimationActions
#####*/
AnimationAction::AnimationAction(AnimationSpriteItem *pItem, AnimationViewerScene *pScene) : m_pScene(pScene), m_pItem(pItem)
{}

void AnimationActionInsert::revert()
{
    if (!m_pItem || !m_pScene)
        return;
    // remove and delete later
    m_pScene->removeItem(m_pItem);
    new DelayedDeleteObject<GraphicsTextureItem>(m_pItem);
}

void AnimationActionRemove::revert()
{
    if (!m_pItem || !m_pScene)
        return;
    // remove and delete later
    m_pScene->addItem(m_pItem);
    m_pItem = nullptr;
}

AnimationActionRemove::~AnimationActionRemove()
{
    if (m_pItem)
        new DelayedDeleteObject<AnimationSpriteItem>(m_pItem);
}

AnimationActionModify::AnimationActionModify(QGraphicsItem::GraphicsItemChange change, const QVariant &value, AnimationSpriteItem *pItem, AnimationViewerScene *pScene)
    : AnimationAction(pItem, pScene), m_Value(value), m_ItemChange(change)
{
}

void AnimationActionModify::revert()
{
    if (!m_pItem)
        return;

    switch (m_ItemChange)
    {
    case QGraphicsItem::ItemPositionChange:
        m_pItem->setPos(m_Value.toPoint());
        break;
    case QGraphicsItem::ItemZValueChange:
        m_pItem->setZValue(m_Value.toReal());
        break;
    case QGraphicsItem::ItemOpacityChange:
        m_pItem->setOpacity(m_Value.toReal());
        break;
    case QGraphicsItem::ItemRotationChange:
        m_pItem->setRotation(m_Value.toReal());
        break;
    case QGraphicsItem::ItemScaleChange:
        m_pItem->setScale(m_Value.toReal());
        break;
    }
}

/*#####
# DatabaseModuleAnimation
#####*/
DatabaseModuleAnimation::DatabaseModuleAnimation(QWidget *pParent) : QWidget(pParent), Ui_DatabaseModuleAnimation(), m_pSpriteDB(nullptr)
{
    setupUi(this);

    // connections
    connect(m_pCurrentFrame, SIGNAL(valueChanged(int)), this, SLOT(_onFrameChanged(int)));
    connect(m_pAniPlayerButton, SIGNAL(clicked()), this, SLOT(_onPlayStopButtonClicked()));
    connect(m_pAniViewer->scene(), SIGNAL(selectionChanged()), this, SLOT(_onSelectionChanged()));
    connect(m_pAniViewer->scene(), SIGNAL(itemChanged(AnimationSpriteItem*, QGraphicsItem::GraphicsItemChange, const QVariant&)),
        this, SLOT(_onItemChanged(AnimationSpriteItem*, QGraphicsItem::GraphicsItemChange, const QVariant&)));
    connect(m_pGridCheckbox, SIGNAL(clicked(bool)), this, SLOT(_onGridCheckboxClicked(bool)));

    connect(m_pSpritePosX, SIGNAL(valueChanged(int)), this, SLOT(_onXChanged(int)));
    connect(m_pSpritePosY, SIGNAL(valueChanged(int)), this, SLOT(_onYChanged(int)));
    connect(m_pSpriteZValue, SIGNAL(valueChanged(int)), this, SLOT(_onZValueChanged(int)));
    connect(m_pSpriteScale, SIGNAL(valueChanged(double)), this, SLOT(_onScaleChanged(double)));
    connect(m_pSpriteOpacity, SIGNAL(valueChanged(double)), this, SLOT(_onOpacityChanged(double)));
    connect(m_pSpriteRotation, SIGNAL(valueChanged(int)), this, SLOT(_onRotationChanged(int)));

    //// add revert action
    //auto newAction = new QAction(this);
    //newAction->setShortcut(tr("Ctrl+Z"));
    //addAction(newAction);
    //connect(newAction, SIGNAL(triggered()), this, SLOT(_onActionRevert()));
}

void DatabaseModuleAnimation::clear()
{
    m_Animation.clear();
    m_pCurrentFrame->setValue(0);
    m_pFrameTime->setValue(0);
    m_pSpriteOpacity->setValue(1);
    m_pSpritePosX->setValue(0);
    m_pSpritePosY->setValue(0);
    m_pSpriteRotation->setValue(0);
    m_pSpriteScale->setValue(1);
    m_pSpriteSpriteID->setValue(0);
    m_pSpriteZValue->setValue(0);
    m_pAniViewer->clear();
}

void DatabaseModuleAnimation::setSpriteDatabase(const DATABASE::SpriteDatabase *pSpriteDB)
{
    m_pSpriteDB = pSpriteDB;
    m_pAniViewer->setSpriteDatabase(m_pSpriteDB);
}

void DatabaseModuleAnimation::setAnimation(const DATABASE::ANIMATION::FrameVector &animation)
{
    _saveCurrentFrame();
    clear();
    m_Animation = animation;
    m_pAniViewer->setAnimation(&m_Animation);
    _setupFrame(0);
}

void DatabaseModuleAnimation::_onGridCheckboxClicked(bool checked)
{
    if (auto pScene = dynamic_cast<AnimationViewerScene*>(m_pAniViewer->scene()))
        pScene->showGrid(checked);
}

void DatabaseModuleAnimation::_onFrameChanged(int index)
{
    _saveCurrentFrame();
    // setup new frame
    _setupFrame(index);
}

void DatabaseModuleAnimation::_setupFrame(uint32 index)
{
    // clear actions
    m_LastActions.clear();

    if (index >= m_Animation.size())
        m_Animation.resize(index+1);
    m_pAniViewer->showFrame(index);
    if (m_pAniViewer->getCurrentFrame() < m_Animation.size())
        m_pFrameTime->setValue(m_Animation.at(m_pAniViewer->getCurrentFrame()).getTimeInMsec());
    else
        m_pFrameTime->setValue(50);
}

void DatabaseModuleAnimation::_onPlayStopButtonClicked()
{
    if (m_pAniViewer->playAnimation())
    {
        m_pAniPlayerButton->setText("start");
        m_pAniViewer->stopAnimation();
        m_pFrame->setEnabled(true);
        _onFrameChanged(0);
    }
    else
    {
        m_pAniPlayerButton->setText("stop");
        m_pAniViewer->startAnimation();
        m_pFrame->setEnabled(false);
    }
}

void DatabaseModuleAnimation::_setupSpriteFrame(AnimationSpriteItem *pItem)
{
    if (!pItem)
        return;
    m_pSpritePosX->setValue(pItem->x());
    m_pSpritePosY->setValue(pItem->y());
    m_pSpriteZValue->setValue(pItem->zValue());
    m_pSpriteScale->setValue(pItem->scale());
    m_pSpriteRotation->setValue(pItem->rotation());
    m_pSpriteOpacity->setValue(pItem->opacity());
    m_pSpriteSpriteID->setValue(pItem->getID());
}

void DatabaseModuleAnimation::_saveCurrentFrame()
{
    uint32 size = m_pAniViewer->getCurrentFrame()+1;
    if (size > m_Animation.size())
        m_Animation.resize(size);

    // save current frame
    DATABASE::ANIMATION::Frame frame;
    auto itemList = m_pAniViewer->scene()->items(Qt::AscendingOrder);
    for (auto pItem : itemList)
    {
        if (auto pSpriteItem = dynamic_cast<AnimationSpriteItem*>(pItem))
        {
            auto sprite = pSpriteItem->toSprite();
            frame.addSprite(sprite);
            frame.setTimeInMsec(m_pFrameTime->value());
        }
    }
    m_Animation.at(m_pAniViewer->getCurrentFrame()) = frame;
}

void DatabaseModuleAnimation::_onSelectionChanged()
{
    auto items = m_pAniViewer->scene()->selectedItems();
    // disable/enable sprite frame
    m_pSpriteFrame->setEnabled(!items.empty());
    if (items.empty())
        return;
    _setupSpriteFrame(dynamic_cast<AnimationSpriteItem*>(items.first()));
}

void DatabaseModuleAnimation::_onXChanged(int value)
{
    if (auto pItem = m_pAniViewer->getSelectedItem())
        pItem->setX(m_pSpritePosX->value());
}

void DatabaseModuleAnimation::_onYChanged(int value)
{
    if (auto pItem = m_pAniViewer->getSelectedItem())
        pItem->setY(m_pSpritePosY->value());
}

void DatabaseModuleAnimation::_onZValueChanged(int value)
{
    if (auto pItem = m_pAniViewer->getSelectedItem())
        pItem->setZValue(value);
}

void DatabaseModuleAnimation::_onScaleChanged(double value)
{
    if (auto pItem = m_pAniViewer->getSelectedItem())
        pItem->setScale(value);
}

void DatabaseModuleAnimation::_onRotationChanged(int value)
{
    if (auto pItem = m_pAniViewer->getSelectedItem())
        pItem->setRotation(value);
}

void DatabaseModuleAnimation::_onOpacityChanged(double value)
{
    if (auto pItem = m_pAniViewer->getSelectedItem())
        pItem->setOpacity(value);
}

void DatabaseModuleAnimation::_onItemChanged(AnimationSpriteItem *pItem, QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (pItem == m_pAniViewer->getSelectedItem())
        _setupSpriteFrame(pItem);

    //if (change == QGraphicsItem::ItemPositionHasChanged)
    //    return;

    //// add action
    //m_LastActions.push_back(std::unique_ptr<AnimationActionModify>(new AnimationActionModify(change, value, pItem, nullptr)));
}
//
//void DatabaseModuleAnimation::_onActionRevert()
//{
//    if (m_LastActions.empty())
//        return;
//
//    m_LastActions.at(m_LastActions.size()-1)->revert();
//    m_LastActions.resize(m_LastActions.size()-2);       // generates a new action, delete it instant after that action
//}
