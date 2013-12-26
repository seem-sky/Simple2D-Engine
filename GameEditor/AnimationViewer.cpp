#include "AnimationViewer.h"
#include "Config.h"
#include "moc_AnimationViewer.h"
#include <QtCore/QMimeData>
#include <QtGui/QKeyEvent>
#include "DelayedDeleteObject.h"
#include "QtGlobal.h"

AnimationSpriteItem::AnimationSpriteItem(const DATABASE::SpriteDatabase* pSpriteDB, uint32 ID) : GraphicsSpriteItem(ID), m_pSpriteDB(pSpriteDB)
{
}

QPixmap AnimationSpriteItem::_getPixmap() const
{
    QPixmap pixmap;
    if (m_pSpriteDB)
    {
        if (auto pSprite = m_pSpriteDB->getOriginalPrototype(getID()))
            createPixmap(Config::get()->getProjectDirectory(), pSprite->getPathName(), pSprite->getTransparencyColor(), pixmap);
    }

    return pixmap;
}

void AnimationSpriteItem::keyPressEvent(QKeyEvent* pEvent)
{
    switch(pEvent->key())
    {
    case Qt::Key_Delete:
        if (scene())
        {
            scene()->removeItem(this);
            if (auto* pScene = dynamic_cast<AnimationViewerScene*>(scene()))
                emit pScene->itemRemoved(this);
        }
        new DelayedDeleteObject<GraphicsSpriteItem>(this);
        break;
    default: GraphicsSpriteItem::keyPressEvent(pEvent);
    }
}

QVariant AnimationSpriteItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    auto pScene = dynamic_cast<AnimationViewerScene*>(scene());
    if (!pScene)
        return value;

    auto result = QGraphicsItem::itemChange(change, value);
    switch (change)
    {
    case QGraphicsItem::ItemPositionHasChanged:
    case QGraphicsItem::ItemPositionChange:
    case QGraphicsItem::ItemZValueChange:
    case QGraphicsItem::ItemOpacityChange:
    case QGraphicsItem::ItemRotationChange:
    case QGraphicsItem::ItemScaleChange:
        emit pScene->itemChanged(this, change, result);
    }
    return result;
}

DATABASE::ANIMATION::Sprite AnimationSpriteItem::toSprite() const
{
    DATABASE::ANIMATION::Sprite sprite;
    sprite.m_Pos = Int32Point(x(), y());
    sprite.m_uiSpriteID = getID();
    sprite.m_uiRotation = rotation();
    sprite.m_Scale = scale();
    sprite.m_Opacity = opacity();
    return sprite;
}

/*#####
# AnimationViewerScene
#####*/
AnimationViewerScene::AnimationViewerScene() : QGraphicsScene(), m_DrawGrid(true)
{}

void AnimationViewerScene::drawForeground(QPainter* pPainter, const QRectF& rect)
{
    if (m_DrawGrid)
        _drawGrid(pPainter, rect);
}

void AnimationViewerScene::showGrid(bool show)
{
    m_DrawGrid = show;
    update();
}

const uint32 GRID_SPACE = 10;
void AnimationViewerScene::_drawGrid(QPainter* pPainter, const QRectF& rect)
{
    // draw 0 lines
    QPainterPath painterPath;
    pPainter->setPen(QColor(255,0,0));
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
    pPainter->drawPath(painterPath);

    // horizontal lines
    pPainter->setPen(QColor(0,0,0));
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
    pPainter->drawPath(linePath);
}

/*#####
# AnimationViewer
#####*/
AnimationViewer::AnimationViewer(QWidget* pParent) : QGraphicsView(pParent), m_pAnimation(nullptr), m_uiCurrentFrameIndex(0), m_pSpriteDB(nullptr), m_Mode(MODE_MODIFY)
{
    setScene(new AnimationViewerScene());
    m_AnimationTimer.setSingleShot(true);
}

void AnimationViewer::clear()
{
    scene()->clear();
    m_pAnimation = nullptr;
    m_uiCurrentFrameIndex = 0;
}

void AnimationViewer::setSpriteDatabase(const DATABASE::SpriteDatabase* pSpriteDB)
{
    m_pSpriteDB = pSpriteDB;
}

void AnimationViewer::setAnimation(const DATABASE::ANIMATION::FrameVector* pAnimation)
{
    m_pAnimation = pAnimation;
    showFrame(0);
}

void AnimationViewer::showFrame(uint32 index)
{
    scene()->clear();
    if (!m_pAnimation || m_pAnimation->empty())
        m_uiCurrentFrameIndex = 0;
    else if (index >= m_pAnimation->size())
        m_uiCurrentFrameIndex = m_pAnimation->size()-1;
    else
    {
        m_uiCurrentFrameIndex = index;
        _setupFrame(m_pAnimation->at(m_uiCurrentFrameIndex));
    }
    repaint();
}

void AnimationViewer::_setupFrame(const DATABASE::ANIMATION::Frame& frame)
{
    if (!m_pSpriteDB)
        return;
    uint32 z = 0;
    for (auto& sprite : frame.getSprites())
    {
        auto pItem = new AnimationSpriteItem(m_pSpriteDB, sprite.m_uiSpriteID);

        // set transformation
        pItem->setZValue(z);
        pItem->setScale(sprite.m_Scale);
        pItem->setOpacity(sprite.m_Opacity);
        pItem->setPos(sprite.m_Pos.x, sprite.m_Pos.y);
        pItem->setRotation(sprite.m_uiRotation);

        addGraphicsSpriteItem(pItem);
        ++z;
    }
}

void AnimationViewer::addGraphicsSpriteItem(AnimationSpriteItem* pItem)
{
    // set item flags
    pItem->setFlag(QGraphicsItem::ItemIsSelectable, m_Mode == MODE_MODIFY);
    pItem->setFlag(QGraphicsItem::ItemIsMovable, m_Mode == MODE_MODIFY);
    pItem->setFlag(QGraphicsItem::ItemIsFocusable, m_Mode == MODE_MODIFY);
    scene()->addItem(pItem);
    if (auto pScene = dynamic_cast<AnimationViewerScene*>(scene()))
        emit pScene->itemAdded(pItem);
}

void AnimationViewer::startAnimation()
{
    connect(&m_AnimationTimer, SIGNAL(timeout()), this, SLOT(_onFrameExpired()));
    _setupTimer();
    setMode(MODE_VIEW);
}

void AnimationViewer::stopAnimation()
{
    m_AnimationTimer.stop();
    disconnect(&m_AnimationTimer, SIGNAL(timeout()), this, SLOT(_onFrameExpired()));
    setMode(MODE_MODIFY);
}

bool AnimationViewer::playAnimation() const
{
    return m_AnimationTimer.isActive();
}

void AnimationViewer::_onFrameExpired()
{
    ++m_uiCurrentFrameIndex;
    if (!m_pAnimation || m_uiCurrentFrameIndex >= m_pAnimation->size())
        showFrame(0);
    else
        showFrame(m_uiCurrentFrameIndex);
    _setupTimer();
}

void AnimationViewer::_setupTimer()
{
    m_AnimationTimer.start(m_pAnimation && m_uiCurrentFrameIndex < m_pAnimation->size() ? m_pAnimation->at(m_uiCurrentFrameIndex).getTimeInMsec() : 1000);
}

void AnimationViewer::setMode(Mode mode)
{
    m_Mode = mode;
    for (auto pItem : scene()->items())
    {
        pItem->setFlag(QGraphicsItem::ItemIsSelectable, mode == MODE_MODIFY);
        pItem->setFlag(QGraphicsItem::ItemIsMovable, mode == MODE_MODIFY);
        pItem->setFlag(QGraphicsItem::ItemIsFocusable, mode == MODE_MODIFY);
    }
}

AnimationSpriteItem* AnimationViewer::getSelectedItem()
{
    auto items = scene()->selectedItems();
    if (items.empty())
        return nullptr;

    return dynamic_cast<AnimationSpriteItem*>(items.first());
}

void AnimationViewer::dragMoveEvent(QDragMoveEvent* pEvent)
{
    pEvent->accept();
}

void AnimationViewer::dragEnterEvent(QDragEnterEvent* pEvent)
{
    pEvent->acceptProposedAction();
}

void AnimationViewer::dropEvent(QDropEvent* pEvent)
{
    if (!m_pSpriteDB)
        return;
    if (auto pSprite = m_pSpriteDB->getOriginalPrototype(pEvent->mimeData()->text().toUInt()))
    {
        auto pItem = new AnimationSpriteItem(m_pSpriteDB, pSprite->getID());
        pItem->setPos(mapToScene(pEvent->pos()).toPoint());
        addGraphicsSpriteItem(pItem);
        pItem->setSelected(true);
    }
}
