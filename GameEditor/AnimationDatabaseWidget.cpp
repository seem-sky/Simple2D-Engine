#include "AnimationDatabaseWidget.h"
#include "moc_AnimationDatabaseWidget.h"
#include <QtWidgets/QScrollBar>

using namespace DATABASE;

AnimationDatabaseWidget::AnimationDatabaseWidget(QWidget *pParent) : DatabaseWidget(pParent), Ui_AnimationWidget()
{
    Ui_AnimationWidget::setupUi(this);
    // setup mod widgets
    m_ModObj.setWidget(m_pAniViewer, MODIFY_RESIZE, QPoint(m_pSpriteList->width()+10, m_pAniPlayerButton->height()+m_pCurrentFrame->height()+m_pFrameTime->height()+20));
    m_ModObj.setWidget(m_pAniPlayerButton, MODIFY_MOVE, QPoint(0, m_pCurrentFrame->height()+m_pFrameTime->height()+15), MODIFY_DIRECTION_HEIGHT);
    m_ModObj.setWidget(m_pCurrentFrame, MODIFY_MOVE, QPoint(0, m_pFrameTime->height()+10), MODIFY_DIRECTION_HEIGHT);
    m_ModObj.setWidget(m_pFrameTime, MODIFY_MOVE, QPoint(0, 5), MODIFY_DIRECTION_HEIGHT);
    m_ModObj.setWidget(m_pCurrentFrameLabel, MODIFY_MOVE, QPoint(0, m_pFrameTime->height()+10), MODIFY_DIRECTION_HEIGHT);
    m_ModObj.setWidget(m_pFrameTimeLabel, MODIFY_MOVE, QPoint(0, 5), MODIFY_DIRECTION_HEIGHT);
    m_ModObj.setWidget(m_pSpriteList, MODIFY_MOVE, QPoint(5, 0), MODIFY_DIRECTION_WIDTH);
    m_ModObj.setWidget(m_pSpriteList, MODIFY_RESIZE, QPoint(0, m_pSpriteFrame->height()+10), MODIFY_DIRECTION_HEIGHT);
    m_ModObj.setWidget(m_pSpriteLabel, MODIFY_MOVE, QPoint(m_pSpriteList->width()- m_pSpriteLabel->width()+5, 0), MODIFY_DIRECTION_WIDTH);
    m_ModObj.setWidget(m_pSpriteFrame, MODIFY_MOVE, QPoint(5, 5));
    connectWidgets();
    m_pAniViewer->setAcceptDrops(true);
    _sceneSelectionChanged();

    connect(m_pAniViewer, SIGNAL(onAnimationStateChanged(bool)), this, SLOT(_onAnimationStateChanged(bool)));
    connect(m_pCurrentFrame, SIGNAL(valueChanged(int)), this, SLOT(_onFrameChange(int)));
    connect(m_pAniViewer, SIGNAL(changedCurrentFrame(uint32, const DATABASE::AnimationPrototype::Frame&)), this, SLOT(_onFrameChange(uint32, const DATABASE::AnimationPrototype::Frame&)));
    connect(m_pFrameTime, SIGNAL(editingFinished()), this, SLOT(_onFrameTimeChange()));
    connect(m_pAniPlayerButton, SIGNAL(clicked()), this, SLOT(_onAnimationPlayerClicked()));
    connect(m_pAniViewer, SIGNAL(onDrop(AnimationView*, uint32, Int32Point)), this, SLOT(_onDrop(AnimationView*, uint32, Int32Point)));
    connect(m_pAniViewer, SIGNAL(onItemChange(ModifyItem*)), this, SLOT(_onSpriteChange(ModifyItem*)));
}

void AnimationDatabaseWidget::setFocus()
{
    m_pSpriteList->fillWithPrototypes();
    m_pAniViewer->setCurrentFrame(m_pCurrentFrame->value());
}

void AnimationDatabaseWidget::_onAnimationStateChanged(bool started)
{
    m_pAniPlayerButton->setText(started ? "Stop" : "Play");
}

AnimationViewScene* AnimationDatabaseWidget::_createNewAnimationViewScene()
{
    if (m_pAniViewer->scene())
        disconnect(m_pAniViewer->scene(), SIGNAL(selectionChanged()), this, SLOT(_sceneSelectionChanged()));
    AnimationViewScene *pScene = new AnimationViewScene(m_pAniViewer);
    m_pAniViewer->setScene(pScene);
    connect(pScene, SIGNAL(selectionChanged()), this, SLOT(_sceneSelectionChanged()));
    _sceneSelectionChanged();
    return pScene;
}

void AnimationDatabaseWidget::_sceneSelectionChanged()
{
    m_pSpritePosX->setValue(0);
    m_pSpritePosY->setValue(0);
    m_pSpriteSpriteID->setValue(0);
    m_pSpriteScale->setValue(100);
    m_pSpriteRotation->setValue(0);
    m_pSpriteOpacity->setValue(100);
    m_pSpriteFrame->setEnabled(false);
    _updateSelectedSprite();
}

void AnimationDatabaseWidget::_updateSelectedSprite()
{
    if (AnimationViewScene *pScene = dynamic_cast<AnimationViewScene*>(m_pAniViewer->scene()))
    {
        if (AnimationViewItem *pItem = pScene->getSelectedItem())
        {
            // disconnect first, so slots wont be call
            disconnect(m_pSpritePosX, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            disconnect(m_pSpritePosY, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            disconnect(m_pSpriteSpriteID, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            disconnect(m_pSpriteScale, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            disconnect(m_pSpriteRotation, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            disconnect(m_pSpriteOpacity, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            m_pSpriteFrame->setEnabled(true);
            m_pSpritePosX->setValue(pItem->scenePos().x());
            m_pSpritePosY->setValue(pItem->scenePos().y());
            m_pSpriteSpriteID->setValue(pItem->getSpriteID());
            m_pSpriteScale->setValue(round(pItem->scale()*100));
            m_pSpriteOpacity->setValue(round(pItem->opacity()*100));
            m_pSpriteRotation->setValue(pItem->rotation());
            connect(m_pSpritePosX, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            connect(m_pSpritePosY, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            connect(m_pSpriteSpriteID, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            connect(m_pSpriteScale, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            connect(m_pSpriteRotation, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
            connect(m_pSpriteOpacity, SIGNAL(valueChanged(int)), this, SLOT(_currentSpriteModifyed(int)));
        }
    }
}

void AnimationDatabaseWidget::_setCurrentFrameValue(uint32 uiFrame)
{
    disconnect(m_pCurrentFrame, SIGNAL(valueChanged(int)), this, SLOT(_onFrameChange(int)));
    m_pCurrentFrame->setValue(uiFrame);
    connect(m_pCurrentFrame, SIGNAL(valueChanged(int)), this, SLOT(_onFrameChange(int)));
}

void AnimationDatabaseWidget::_onSpriteChange(ModifyItem *pItem)
{
    updateItem();
    _updateSelectedSprite();
}

void AnimationDatabaseWidget::_onDrop(AnimationView *pTarget, uint32 uiID, Int32Point pos)
{
    ConstSpritePrototypePtr proto;
    QPixmap pixmap;
    if (!m_pDBChanger || !m_pSpriteDB->getItem(uiID, proto))
        return;
    AnimationViewItem *pItem = new AnimationViewItem(proto);
    pItem->setFlag(QGraphicsItem::ItemIsMovable, !m_pAniViewer->hasActiveAnimation());
    pItem->setFlag(QGraphicsItem::ItemIsSelectable, !m_pAniViewer->hasActiveAnimation());
    pItem->setFlag(QGraphicsItem::ItemIsFocusable, !m_pAniViewer->hasActiveAnimation());
    m_pAniViewer->scene()->addItem(pItem);
    pItem->moveBy(pos.x, pos.y);
    updateItem();
}

void AnimationDatabaseWidget::_onFrameChange(int value)
{
    m_pFrameTime->setValue(0);
    m_pAniViewer->setCurrentFrame(value);
}

void AnimationDatabaseWidget::_onFrameChange(uint32 uiFrame, const DATABASE::AnimationPrototype::Frame &curFrame)
{
    m_pFrameTime->setValue(curFrame.m_uiMsecTime);
    if (m_pCurrentFrame->value() != uiFrame)
        _setCurrentFrameValue(0);
}

void AnimationDatabaseWidget::_onFrameTimeChange()
{
    updateItem();
}

bool AnimationDatabaseWidget::getItemFromWidgets(AnimationPrototypePtr &proto)
{
    if (!m_pDBChanger || !m_pDBChanger->getItem(getCurrentID(), proto))
        return false;
    if (!DatabaseWidget::getItemFromWidgets(proto) || !m_pAniViewer->scene())
        return false;

    // store only current frame, get the other from db changer
    AnimationPrototype::Frame frame;
    frame.m_uiMsecTime = m_pFrameTime->value();
    QList<QGraphicsItem*> itemList = m_pAniViewer->scene()->items(Qt::AscendingOrder);
    for (QList<QGraphicsItem*>::const_iterator itr = itemList.begin(); itr != itemList.end(); ++itr)
    {
        if (AnimationViewItem *pItem = dynamic_cast<AnimationViewItem*>(*itr))
        {
            AnimationPrototype::Sprite sprite;
            sprite.m_Pos = Int32Point(pItem->pos().x(), pItem->pos().y());
            sprite.m_uiSpriteID = pItem->getSpriteID();
            sprite.m_uiScale = round(pItem->scale()*100);
            sprite.m_uiRotation = pItem->rotation();
            sprite.m_uiOpacity = round(pItem->opacity()*100);
            frame.m_Sprites.push_back(sprite);
        }
    }
    proto->setFrame(m_pCurrentFrame->value(), frame);
    return true;
}

bool AnimationDatabaseWidget::setWidgetsFromPrototype(const AnimationPrototypePtr &proto)
{
    m_pAniViewer->createNewAnimationScene();
    if (!DatabaseWidget::setWidgetsFromPrototype(proto) || !m_pSpriteDB)
        return false;

    m_pFrameTime->setValue(0);
    m_pCurrentFrame->setValue(0);
    m_pAniViewer->setCurrentAnimation(proto->getID());
    return true;
}

void AnimationDatabaseWidget::_onAnimationPlayerClicked()
{
    if (m_pAniViewer->hasActiveAnimation())
        m_pAniViewer->stopCurrentAnimation();
    else
        m_pAniViewer->playCurrentAnimation();
}

void AnimationDatabaseWidget::_currentSpriteModifyed(int value)
{
    if (AnimationViewScene *pScene = dynamic_cast<AnimationViewScene*>(m_pAniViewer->scene()))
    {
        if (AnimationViewItem *pItem = pScene->getSelectedItem())
        {
            pItem->setPos(m_pSpritePosX->value(), m_pSpritePosY->value());
            pItem->setScale(qreal(m_pSpriteScale->value())/100);
            pItem->setOpacity(qreal(m_pSpriteOpacity->value())/100);
            pItem->setRotation(m_pSpriteRotation->value());
            _onSpriteChange(NULL);
        }
    }
}

void AnimationDatabaseWidget::setDB(const DATABASE::AnimationDatabasePtr &pDB)
{
    m_pAniViewer->setAnimationDB(getDBChanger());
    DatabaseWidget::setDB(pDB);
}