#include "VisualViewer.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
#include "QtGlobal.h"
#include "Config.h"
#include "moc_VisualViewer.h"

using namespace DATABASE;

/*#####
# VisualSpriteItem
#####*/
VisualSpriteItem::VisualSpriteItem(const DatabaseMgr& DBMgr, uint32 ID) : QGraphicsItem(), m_DBMgr(DBMgr), m_ID(ID)
{
    if (!m_ID)
        assert("ID not allowed to be NULL!");
}

QPixmap VisualSpriteItem::_getPixmap() const
{
    QPixmap pixmap;
    if (auto pSprite = m_DBMgr.getSpriteDatabase()->getOriginalPrototype(getID()))
        createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pSprite, pixmap);

    return pixmap;
}

void VisualSpriteItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    auto pixmap = _getPixmap();
    setTransformOriginPoint(pixmap.rect().center());
    pPainter->drawPixmap(0, 0, pixmap);
}

QRectF VisualSpriteItem::boundingRect() const
{
    auto pixmap = _getPixmap();
    return pixmap.rect();
}

uint32 VisualSpriteItem::getID() const
{
    return m_ID;
}

/*#####
# VisualViewerScene
#####*/
VisualViewerScene::VisualViewerScene() : QGraphicsScene(), m_DrawGrid(true)
{}

void VisualViewerScene::drawForeground(QPainter* pPainter, const QRectF& rect)
{
    if (m_DrawGrid)
        _drawGrid(pPainter, rect);
}

void VisualViewerScene::showGrid(bool show)
{
    m_DrawGrid = show;
    update();
}

const uint32 GRID_SPACE = 10;
void VisualViewerScene::_drawGrid(QPainter* pPainter, const QRectF& rect)
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
# VisualViewer
#####*/
VisualViewer::VisualViewer(const DatabaseMgr& DBMgr, QWidget *pParent)
    : QGraphicsView(pParent), m_uiCurrentFrameIndex(0), m_DBMgr(DBMgr), m_WorldObjectID(0), m_AnimationEntry(0), m_DoAnimation(false)
{
    setScene(new VisualViewerScene());
    scene()->setParent(this);
    m_AnimationTimer.setSingleShot(true);
}

void VisualViewer::clear()
{
    scene()->clear();
    m_WorldObjectID = 0;
    m_AnimationEntry = 0;
    m_uiCurrentFrameIndex = 0;
    m_DoAnimation = false;
}

void VisualViewer::showVisual()
{
    scene()->clear();

    auto animationInfo = getAnimationInfo();
    if (!animationInfo.isValid())
        return;

    switch (animationInfo.m_VisualType)
    {
    case WORLD_OBJECT::AnimationInfo::VisualType::SPRITE:
        _setupSprite(animationInfo.m_ID);
        break;
    case WORLD_OBJECT::AnimationInfo::VisualType::ANIMATION:
        _setupAnimationFrame(animationInfo.m_ID, 0);
        break;
    }
    repaint();
}

void VisualViewer::_setupSprite(uint32 ID) const
{
    scene()->clear();
    if (auto pSprite = m_DBMgr.getSpriteDatabase()->getOriginalPrototype(ID))
        scene()->addItem(new VisualSpriteItem(m_DBMgr, ID));
}

void VisualViewer::_setupAnimationFrame(uint32 ID, uint32 frameIndex)
{
    scene()->clear();
    if (auto pAnimation = m_DBMgr.getAnimationDatabase()->getOriginalPrototype(ID))
    {
        // stop animation if empty
        uint32 animationCount = pAnimation->getFrameCount();
        if (animationCount == 0)
        {
            stopAnimation();
            return;
        }

        if (animationCount <= frameIndex)
            frameIndex = animationCount-1;

        ANIMATION::Frame frame;
        pAnimation->getFrame(frameIndex, frame);
        uint32 z = 0;
        for (auto& sprite : frame.getSprites())
        {
            auto pItem = new VisualSpriteItem(m_DBMgr, sprite.m_uiSpriteID);

            // set transformation
            pItem->setZValue(z);
            pItem->setScale(sprite.m_Scale);
            pItem->setOpacity(sprite.m_Opacity);
            pItem->setPos(sprite.m_Pos.x, sprite.m_Pos.y);
            pItem->setRotation(sprite.m_uiRotation);

            scene()->addItem(pItem);
            ++z;
        }

        m_AnimationTimer.start(frame.getTimeInMsec());
    }
}

DATABASE::WORLD_OBJECT::AnimationInfo VisualViewer::getAnimationInfo() const
{
    if (auto pWorldObject = m_DBMgr.getWorldObjectDatabase()->getOriginalPrototype(m_WorldObjectID))
    {
        if (pWorldObject->getAnimationCount() <= m_AnimationEntry)
            return pWorldObject->getAnimationInfo(m_AnimationEntry);
    }
    return DATABASE::WORLD_OBJECT::AnimationInfo();
}

void VisualViewer::startAnimation()
{
    connect(&m_AnimationTimer, SIGNAL(timeout()), this, SLOT(_onFrameExpired()));
    m_DoAnimation = true;
    m_uiCurrentFrameIndex = 0;
    showVisual();
}

void VisualViewer::stopAnimation()
{
    m_AnimationTimer.stop();
    m_DoAnimation = false;
    disconnect(&m_AnimationTimer, SIGNAL(timeout()), this, SLOT(_onFrameExpired()));
}

bool VisualViewer::isAnimationActive() const
{
    return m_DoAnimation;
}

void VisualViewer::_onFrameExpired()
{
    auto animationInfo = getAnimationInfo();
    if (!animationInfo.isValid() || animationInfo.m_VisualType != WORLD_OBJECT::AnimationInfo::VisualType::ANIMATION || !isAnimationActive())
        stopAnimation();

    _setupAnimationFrame(animationInfo.m_ID, ++m_uiCurrentFrameIndex);
}

void VisualViewer::dragMoveEvent(QDragMoveEvent* pEvent)
{
    pEvent->accept();
}

void VisualViewer::dragEnterEvent(QDragEnterEvent* pEvent)
{
    pEvent->acceptProposedAction();
}

void VisualViewer::dropEvent(QDropEvent* pEvent)
{
    auto texts = pEvent->mimeData()->text().split(",");
    if (texts.count() >= 2)
    {
        uint32 ID = texts.at(0).toUInt();
        auto DBtype = static_cast<DatabaseType>(texts.at(1).toUInt());
        if (ID && (DBtype == DatabaseType::SPRITE_DATABASE || DBtype == DatabaseType::ANIMATION_DATABASE))
        {
            clear();
            showVisual();
        }
    }
}
