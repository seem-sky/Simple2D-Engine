#include "VisualViewer.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QDropEvent>
#include <QtCore/QMimeData>
#include "QtGlobal.h"
#include "Config.h"
#include "moc_VisualViewer.h"

using namespace DATABASE;
using namespace PROTOTYPE;

/*#####
# VisualSpriteItem
#####*/
VisualSpriteItem::VisualSpriteItem(const DatabaseMgr& DBMgr, uint32 ID) : QGraphicsItem(), m_DBMgr(DBMgr), m_ID(ID)
{
    if (!m_ID)
        std::runtime_error("ID not allowed to be NULL!");
}

void VisualSpriteItem::_getPixmap(QPixmap& pixmap) const
{
    if (auto pSprite = m_DBMgr.getSpriteDatabase()->getOriginalPrototype(getID()))
        createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pSprite, pixmap);
}

void VisualSpriteItem::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget)
{
    QPixmap pixmap;
    _getPixmap(pixmap);
    pPainter->drawPixmap(0, 0, pixmap);
}

QRectF VisualSpriteItem::boundingRect() const
{
    QPixmap pixmap;
    _getPixmap(pixmap);
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
VisualViewer::VisualViewer(QWidget *pParent)
    : QGraphicsView(pParent), m_uiCurrentFrameIndex(0), m_pDBMgr(nullptr), m_AnimationEntry(0), m_DoAnimation(false),
    m_VisualType(MODULE::ANIMATION::VisualType::SPRITE)
{
    setScene(new VisualViewerScene());
    scene()->setParent(this);
    m_AnimationTimer.setSingleShot(true);
}

void VisualViewer::setDatabaseManager(const DATABASE::DatabaseMgr* pDBMgr)
{
    m_pDBMgr = pDBMgr;
    if (m_pDBMgr)
        showVisual();
}

void VisualViewer::clear()
{
    scene()->clear();
    m_AnimationEntry = 0;
	m_VisualType = MODULE::ANIMATION::VisualType::SPRITE;
    m_uiCurrentFrameIndex = 0;
    m_DoAnimation = false;
}

void VisualViewer::showVisual()
{
    switch (m_VisualType)
    {
	case MODULE::ANIMATION::VisualType::SPRITE:
        _setupSprite();
        break;
	case MODULE::ANIMATION::VisualType::ANIMATION:
        _setupAnimationFrame(0);
        break;
    }
}

void VisualViewer::_setupSprite() const
{
    scene()->clear();
    if (!m_pDBMgr)
        return;
    if (auto pSprite = m_pDBMgr->getSpriteDatabase()->getOriginalPrototype(m_AnimationEntry))
        scene()->addItem(new VisualSpriteItem(*m_pDBMgr, m_AnimationEntry));
}

int VisualViewer::_setupAnimationFrame(uint32 frameIndex)
{
    scene()->clear();
    if (m_pDBMgr)
    {
        if (auto pAnimation = m_pDBMgr->getAnimationDatabase()->getOriginalPrototype(m_AnimationEntry))
        {
            // stop animation if empty
            uint32 animationCount = pAnimation->getFrameCount();
            if (animationCount <= 1)
            {
                stopAnimation();
                frameIndex = 0;
            }

            if (animationCount <= frameIndex)
                frameIndex = 0;

            ANIMATION::Frame frame;
            pAnimation->getFrame(frameIndex, frame);
            uint32 z = 0;
            for (auto& sprite : frame.getSprites())
            {
                auto pItem = new VisualSpriteItem(*m_pDBMgr, sprite.m_uiSpriteID);

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
    return frameIndex;
}

void VisualViewer::startAnimation()
{
	if (m_VisualType == MODULE::ANIMATION::VisualType::ANIMATION)
    {
        connect(&m_AnimationTimer, SIGNAL(timeout()), this, SLOT(_onFrameExpired()));
        m_DoAnimation = true;
        m_uiCurrentFrameIndex = 0;
    }
    else
        stopAnimation();
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
	if (m_VisualType != MODULE::ANIMATION::VisualType::ANIMATION || !isAnimationActive())
        stopAnimation();

    m_uiCurrentFrameIndex = _setupAnimationFrame(m_uiCurrentFrameIndex+1);
}

void VisualViewer::dragMoveEvent(QDragMoveEvent* pEvent)
{
    QGraphicsView::dragMoveEvent(pEvent);
    pEvent->accept();
}

void VisualViewer::dragEnterEvent(QDragEnterEvent* pEvent)
{
    QGraphicsView::dragEnterEvent(pEvent);
    pEvent->acceptProposedAction();
}

void VisualViewer::dropEvent(QDropEvent* pEvent)
{
    auto texts = pEvent->mimeData()->text().split(",");
    if (texts.count() >= 2)
    {
        uint32 ID = texts.at(0).toUInt();
        auto DBtype = static_cast<DatabaseType>(texts.at(1).toUInt());
        if (ID)
        {
            switch (DBtype)
            {
            case DATABASE::DatabaseType::SPRITE_DATABASE:
				setAnimation(ID, MODULE::ANIMATION::VisualType::SPRITE);
                break;
            case DATABASE::DatabaseType::ANIMATION_DATABASE:
				setAnimation(ID, MODULE::ANIMATION::VisualType::ANIMATION);
                break;
            default:
                throw std::runtime_error("Invalid database type.");
            }
            pEvent->acceptProposedAction();
        }
    }
    pEvent->ignore();
}

void VisualViewer::setAnimation(uint32 animationEntry, MODULE::ANIMATION::VisualType type)
{
    clear();
    m_AnimationEntry = animationEntry;
    m_VisualType = type;
    startAnimation();
}
