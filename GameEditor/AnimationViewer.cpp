#include "AnimationViewer.h"
#include "QtGlobal.h"
#include "Config.h"
#include "moc_AnimationViewer.h"
#include <QtWidgets/QStyleOption>
#include <QtGui/QKeyEvent>
#include <QtCore/QMimeData>
#include "DelayedDeleteObject.h"

/*#####
# GraphicsSpriteItem
#####*/
GraphicsSpriteItem::GraphicsSpriteItem(const DATABASE::SpritePrototype *pSpritePrototype) : QGraphicsItem(), m_pSpritePrototype(pSpritePrototype)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

void GraphicsSpriteItem::keyPressEvent(QKeyEvent *pEvent)
{
    switch(pEvent->key())
    {
    case Qt::Key_Up: moveBy(0, -1); break;
    case Qt::Key_Down: moveBy(0, 1); break;
    case Qt::Key_Left: moveBy(-1, 0); break;
    case Qt::Key_Right: moveBy(1, 0); break;
    case Qt::Key_Delete:
        if (scene())
        {
            scene()->removeItem(this);
            if (auto *pScene = dynamic_cast<AnimationViewerScene*>(scene()))
                emit pScene->itemRemoved(this);
        }
        new DelayedDeleteObject<GraphicsSpriteItem>(this);
    default: return;
    }
}

void GraphicsSpriteItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    auto pixmap = _getPixmap();
    pPainter->drawPixmap(0, 0, pixmap);
    // draw selection box
    if (isSelected())
        _hightlightSelection(pPainter, pOption);
}

void GraphicsSpriteItem::_hightlightSelection(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption)
{
    // from QGraphicsItem.cpp
    const QRectF murect = pPainter->transform().mapRect(QRectF(0, 0, 1, 1));
    if (qFuzzyCompare(qMax(murect.width(), murect.height()) + 1, 1))
        return;

    const QRectF mbrect = pPainter->transform().mapRect(boundingRect());
    if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
        return;

    qreal itemPenWidth = 1.0;
    const qreal pad = itemPenWidth / 2;

    const qreal penWidth = 0; // cosmetic pen

    const QColor fgcolor = pOption->palette.windowText().color();
    const QColor bgcolor( // ensure good contrast against fgcolor
        fgcolor.red()   > 127 ? 0 : 255,
        fgcolor.green() > 127 ? 0 : 255,
        fgcolor.blue()  > 127 ? 0 : 255);

    pPainter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
    pPainter->setBrush(Qt::NoBrush);
    pPainter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));

    pPainter->setPen(QPen(pOption->palette.windowText(), 0, Qt::DashLine));
    pPainter->setBrush(Qt::NoBrush);
    pPainter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));
}

QRectF GraphicsSpriteItem::boundingRect() const
{
    auto pixmap = _getPixmap();
    return pixmap.rect();
}

QPixmap GraphicsSpriteItem::_getPixmap() const
{
    QPixmap pixmap;
    createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), m_pSpritePrototype, pixmap);
    return pixmap;
}

uint32 GraphicsSpriteItem::getSpriteID() const
{
    if (m_pSpritePrototype)
        return m_pSpritePrototype->getID();
    return 0;
}

QVariant GraphicsSpriteItem::itemChange(GraphicsItemChange change, const QVariant &value)
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

DATABASE::ANIMATION::Sprite GraphicsSpriteItem::toSprite() const
{
    DATABASE::ANIMATION::Sprite sprite;
    sprite.m_Pos = Int32Point(x(), y());
    sprite.m_uiSpriteID = getSpriteID();
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

void AnimationViewerScene::drawForeground(QPainter *pPainter, const QRectF &rect)
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
void AnimationViewerScene::_drawGrid(QPainter *pPainter, const QRectF &rect)
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
AnimationViewer::AnimationViewer(QWidget *pParent) : QGraphicsView(pParent), m_pAnimation(nullptr), m_uiCurrentFrameIndex(0), m_pSpriteDB(nullptr), m_Mode(MODE_MODIFY)
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

void AnimationViewer::setSpriteDatabase(const DATABASE::SpriteDatabase *pSpriteDB)
{
    m_pSpriteDB = pSpriteDB;
}

void AnimationViewer::setAnimation(const DATABASE::ANIMATION::FrameVector *pAnimation)
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

void AnimationViewer::_setupFrame(const DATABASE::ANIMATION::Frame &frame)
{
    if (!m_pSpriteDB)
        return;
    uint32 z = 0;
    for (auto &sprite : frame.getSprites())
    {
        if (auto pPrototype = m_pSpriteDB->getOriginalPrototype(sprite.m_uiSpriteID))
        {
            auto pItem = new GraphicsSpriteItem(pPrototype);

            // set transformation
            pItem->setZValue(z);
            pItem->setScale(sprite.m_Scale);
            pItem->setOpacity(sprite.m_Opacity);
            pItem->setPos(sprite.m_Pos.x, sprite.m_Pos.y);
            pItem->setRotation(sprite.m_uiRotation);

            addGraphicsSpriteItem(pItem);
        }
        ++z;
    }
}

void AnimationViewer::addGraphicsSpriteItem(GraphicsSpriteItem *pItem)
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

GraphicsSpriteItem* AnimationViewer::getSelectedItem()
{
    auto items = scene()->selectedItems();
    if (items.empty())
        return nullptr;

    return dynamic_cast<GraphicsSpriteItem*>(items.first());
}

void AnimationViewer::dragMoveEvent(QDragMoveEvent *pEvent)
{
    pEvent->accept();
}

void AnimationViewer::dragEnterEvent(QDragEnterEvent *pEvent)
{
    pEvent->acceptProposedAction();
}

void AnimationViewer::dropEvent(QDropEvent *pEvent)
{
    if (!m_pSpriteDB)
        return;
    if (auto pSprite = m_pSpriteDB->getOriginalPrototype(pEvent->mimeData()->text().toUInt()))
    {
        auto pItem = new GraphicsSpriteItem(pSprite);
        pItem->setPos(mapToScene(pEvent->pos()).toPoint());
        addGraphicsSpriteItem(pItem);
        pItem->setSelected(true);
    }
}
