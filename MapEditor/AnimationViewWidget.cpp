#include "AnimationViewWidget.h"
#include "moc_AnimationViewWidget.h"
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QTreeWidget>
#include <QtGui/QKeyEvent>
#include <QtGui/QGraphicsSceneWheelEvent>
#include <QtGui/QTransform>
#include "AnimationDatabaseWidget.h"

using namespace DATABASE;

/*#####
# AnimationViewItem
#####*/
AnimationViewItem::AnimationViewItem(DATABASE::ConstSpritePrototypePtr pProto) : ModifyItem(), m_uiID(0)
{
    if (pProto)
    {
        QPixmap pixmap;
        if (createPixmapFromTexturePrototype(pProto, pixmap))
        {
            setPixmap(pixmap);
            QSize pixmapSize = pixmap.size();
            setOffset(-pixmapSize.width()/2, -pixmapSize.height()/2);
        }
        m_uiID = pProto->getID();
    }
}

void AnimationViewItem::keyPressEvent(QKeyEvent *pEvent)
{
    switch(pEvent->key())
    {
    case Qt::Key_Plus:
        setZValue(zValue()+0.1f);
        if (zValue() < 0.5f)
            setZValue(0.5f);
        sendUpdate();
        break;
    case Qt::Key_Minus:
        setZValue(zValue()-0.1f);
        sendUpdate();
        break;
    default: ModifyItem::keyPressEvent(pEvent); break;
    }
}

void AnimationViewItem::wheelEvent(QGraphicsSceneWheelEvent *pEvent)
{
    setZValue(zValue() - pEvent->delta() / 120 * 0.1f);
    if (zValue() < 0.5f)
        setZValue(0.5f);
    sendUpdate();
}

/*#####
# AnimationViewWidget
#####*/
AnimationView::AnimationView(QWidget *pParent) : ModifyView(pParent), m_uiCurrentFrame(0), m_uiCurrentAnimationID(0), m_ActiveAnimation(false),
m_ContinuesAnimation(false), m_DrawGrid(true), m_DrawPreviousFrame(true), m_uiSpeedModifyer(100), m_Editable(true)
{
    setAcceptDrops(true);
    setDragMode(QGraphicsView::ScrollHandDrag);
    createNewAnimationScene();
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(_onUpdateAnimation()));
}

void AnimationView::dragMoveEvent(QDragMoveEvent *pEvent)
{
    pEvent->accept();
}

void AnimationView::dragEnterEvent(QDragEnterEvent *pEvent)
{
    pEvent->acceptProposedAction();
}

void AnimationView::dropEvent(QDropEvent *pEvent)
{
    if (QTreeWidget *pWidget = dynamic_cast<QTreeWidget*>(pEvent->source()))
    {
        if (QTreeWidgetItem *pItem = dynamic_cast<QTreeWidgetItem*>(pWidget->currentItem()))
        {
            uint32 uiID = pItem->text(0).toUInt();
            QPoint scenePos = mapToScene(pEvent->pos()).toPoint();
            emit onDrop(this, uiID, Point<int32>(scenePos.x(), scenePos.y()));
        }
    }
}

void AnimationView::fixSceneRect(bool fix)
{
    if (fix)
    {
        QSize viewportSize = viewport()->size();
        setSceneRect(-viewportSize.width() /3, -viewportSize.height() /3, viewportSize.width(), viewportSize.height());
    }
    else
        setSceneRect(QRectF());
}

void AnimationView::setCurrentAnimation(uint32 uiAnimationID)
{
    m_uiCurrentAnimationID = uiAnimationID;
    setCurrentFrame(0);
}

bool AnimationView::_drawFrame(const DATABASE::AnimationPrototype::Frame &frame)
{
    for (uint32 i = 0; i < frame.m_Sprites.size(); ++i)
    {
        ConstSpritePrototypePtr proto;
        if (_getSpritePrototype(frame.m_Sprites.at(i).m_uiSpriteID, proto))
        {
            AnimationViewItem *pItem = new AnimationViewItem(proto);
            scene()->addItem(pItem);
            pItem->setFlag(QGraphicsItem::ItemIsMovable, !m_ActiveAnimation && isEditable());
            pItem->setFlag(QGraphicsItem::ItemIsSelectable, !m_ActiveAnimation && isEditable());
            pItem->setFlag(QGraphicsItem::ItemIsFocusable, !m_ActiveAnimation && isEditable());
            pItem->setPos(QPoint(frame.m_Sprites.at(i).m_Pos.x, frame.m_Sprites.at(i).m_Pos.y));
            pItem->setScale(qreal(frame.m_Sprites.at(i).m_uiScale)/100);
            pItem->setRotation(frame.m_Sprites.at(i).m_uiRotation);
            pItem->setOpacity(qreal(frame.m_Sprites.at(i).m_uiOpacity)/100);
        }
    }
    return true;
}

bool AnimationView::setCurrentFrame(uint32 uiFrame)
{
    m_uiCurrentFrame = uiFrame;
    createNewAnimationScene();
    bool success = true;
    ConstAnimationPrototypePtr proto;
    AnimationPrototype::Frame frame;
    if (!_getAnimationPrototype(m_uiCurrentAnimationID, proto) || !proto->getFrame(m_uiCurrentFrame, frame) || !_drawFrame(frame))
        success = false;

    emit changedCurrentFrame(m_uiCurrentFrame, frame);
    if (m_uiCurrentFrame > 0 && proto->getFrame(m_uiCurrentFrame-1, frame))
        dynamic_cast<AnimationViewScene*>(scene())->setPreviousFrame(frame);
    return success;
}

void AnimationView::createNewAnimationScene()
{
    if (AnimationViewScene *pScene = dynamic_cast<AnimationViewScene*>(scene()))
    {
        pScene->clear();
        pScene->setPreviousFrame(AnimationPrototype::Frame());
        viewport()->update();
    }
    else
    {
        pScene = new AnimationViewScene(this);
        if (!m_DrawGrid)
            pScene->setGridDraw(false);
        if (!m_DrawPreviousFrame)
            pScene->setPreviousFrameDraw(false);
        setScene(pScene);
        connect(pScene, SIGNAL(selectionChanged()), parent(), SLOT(_sceneSelectionChanged()));
    }
}

void AnimationView::playCurrentAnimation()
{
    m_ActiveAnimation = true;
    if (AnimationViewScene *pScene = dynamic_cast<AnimationViewScene*>(scene()))
    {
        pScene->setGridDraw(false);
        pScene->setPreviousFrameDraw(false);
    }
    fixSceneRect(true);
    _onUpdateAnimation(true);
    emit onAnimationStateChanged(true);
}

void AnimationView::stopCurrentAnimation()
{
    m_Timer.stop();
    // prevent endless recursion, check if it has more than 1 frame
    if (m_ContinuesAnimation && m_uiCurrentFrame > 1)
        playCurrentAnimation();
    else
    {
        m_ActiveAnimation = false;
        fixSceneRect(false);
        setCurrentFrame(0);
        if (AnimationViewScene *pScene = dynamic_cast<AnimationViewScene*>(scene()))
        {
            pScene->setGridDraw(isGridDrawActiv());
            pScene->setPreviousFrameDraw(isPreviousFrameDrawActiv());
        }
        emit onAnimationStateChanged(false);
    }
}

void AnimationView::_onUpdateAnimation(bool firstTime)
{
    if (!m_ActiveAnimation)
        return;
    if (setCurrentFrame(firstTime ? 0 : m_uiCurrentFrame+1))
    {
        ConstAnimationPrototypePtr proto;
        AnimationPrototype::Frame frame;
        if (_getAnimationPrototype(m_uiCurrentAnimationID, proto) && proto->getFrame(m_uiCurrentFrame, frame))
            m_Timer.start(frame.m_uiMsecTime * 100 / m_uiSpeedModifyer);
    }
    else
        stopCurrentAnimation();
}

void AnimationView::setGridDraw(bool draw)
{
    m_DrawGrid = draw;
    if (AnimationViewScene *pScene = dynamic_cast<AnimationViewScene*>(scene()))
    {
        pScene->setGridDraw(draw);
        viewport()->update();
    }
}

void AnimationView::setPreviousFrameDraw(bool draw)
{
    m_DrawPreviousFrame = draw;
    if (AnimationViewScene *pScene = dynamic_cast<AnimationViewScene*>(scene()))
    {
        pScene->setPreviousFrameDraw(draw);
        viewport()->update();
    }
}

/*#####
# AnimationViewDBChanger
#####*/
bool AnimationViewDBChanger::_getAnimationPrototype(uint32 uiID, DATABASE::ConstAnimationPrototypePtr &proto)
{
    if (m_pAnimationDB && m_pAnimationDB->getPrototype(uiID, proto))
        return true;
    return false;
}

bool AnimationViewDBChanger::_getSpritePrototype(uint32 uiID, DATABASE::ConstSpritePrototypePtr &proto)
{
    if (m_pSpriteDB && m_pSpriteDB->getPrototype(uiID, proto))
        return true;
    return false;
}

/*#####
# AnimationViewDB
#####*/
bool AnimationViewDB::_getAnimationPrototype(uint32 uiID, DATABASE::ConstAnimationPrototypePtr &proto)
{
    if (m_pAnimationDB && m_pAnimationDB->getPrototype(uiID, proto))
        return true;
    return false;
}

bool AnimationViewDB::_getSpritePrototype(uint32 uiID, DATABASE::ConstSpritePrototypePtr &proto)
{
    if (m_pSpriteDB && m_pSpriteDB->getPrototype(uiID, proto))
        return true;
    return false;
}

/*#####
# AnimationViewScene
#####*/
AnimationViewScene::AnimationViewScene(AnimationView *pParent) : QGraphicsScene(pParent), m_DrawGrid(true), m_DrawPreviousFrame(true)
{
    connect(this, SIGNAL(changed(const QList<QRectF>&)), this, SLOT(_onSceneChanged()));
}

void AnimationViewScene::_drawPreviousFrame(QPainter *painter, const QRectF &rect)
{
    if (!parent() || !parent()->parent())
        return;

    if (AnimationDatabaseWidget *pWidget = dynamic_cast<AnimationDatabaseWidget*>(parent()->parent()))
    {
        DATABASE::ConstSpriteDatabaseChangerPtr pDB = pWidget->getSpriteDB();
        if (!pDB)
            return;

        DATABASE::ConstSpritePrototypePtr proto;
        QPixmap pixmap;
        for (uint32 i = 0; i < m_PreviousFrame.m_Sprites.size(); ++i)
        {
            DATABASE::AnimationPrototype::Sprite sprite = m_PreviousFrame.m_Sprites.at(i);
            if (!pDB->getPrototype(sprite.m_uiSpriteID, proto) || !createPixmapFromTexturePrototype(proto, pixmap))
                continue;
            painter->setOpacity(0.5f*(qreal(sprite.m_uiOpacity)/100));
            painter->rotate(sprite.m_uiRotation);
            qreal scale = qreal(sprite.m_uiScale)/100;
            painter->scale(scale, scale);
            painter->drawPixmap(sprite.m_Pos.x - pixmap.width()/2, sprite.m_Pos.y - pixmap.width()/2, pixmap.width(), pixmap.height(), pixmap);
            painter->save();
        }
    }
}

const uint32 GRID_SPACE = 10;

void AnimationViewScene::_drawGrid(QPainter *painter, const QRectF &rect)
{
    // draw 0 lines
    QPainterPath painterPath;
    painter->setPen(QColor(255,0,0));
    if (rect.y() <= 0 && rect.y() + rect.height() >= 0)
    {
        painterPath.moveTo(rect.x(), 0);
        painterPath.lineTo(rect.x()+rect.width(), 0);
    }
    if (rect.x() <= 0 && rect.x() + rect.width() >= 0)
    {
        painterPath.moveTo(0, rect.y());
        painterPath.lineTo(0, rect.y()+rect.height());
    }
    painter->drawPath(painterPath);
    painter->save();
    // horizontal lines
    painter->setPen(QColor(0,0,0));
    QPainterPath linePath;
    for (int i = int(rect.y()/GRID_SPACE)*GRID_SPACE; i < rect.y() + rect.height(); i+=GRID_SPACE)
    {
        if (i == 0)
            continue;
        linePath.moveTo(rect.x(), i);
        linePath.lineTo(rect.x()+rect.width(), i);
    }

    // vertical lines
    for (int i = int(rect.x()/GRID_SPACE)*GRID_SPACE; i < rect.x() + rect.width(); i+=GRID_SPACE)
    {
        if (i == 0)
            continue;
        linePath.moveTo(i, rect.y());
        linePath.lineTo(i, rect.y()+rect.height());
    }
    painter->drawPath(linePath);
}

void AnimationViewScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (m_DrawGrid)
        _drawGrid(painter, rect);
    if (m_DrawPreviousFrame)
        _drawPreviousFrame(painter, rect);
}

AnimationViewItem* AnimationViewScene::getSelectedItem()
{
    QList<QGraphicsItem*> items = selectedItems();
    if (items.isEmpty())
        return NULL;
    if (AnimationViewItem *pItem = dynamic_cast<AnimationViewItem*>(items.first()))
        return pItem;
    return NULL;
}
