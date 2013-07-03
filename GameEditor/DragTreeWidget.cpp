#include "DragTreeWidget.h"
#include "QtGlobal.h"
#include "AnimationDatabaseWidget.h"
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include "moc_DragTreeWidget.h"
#include "Config.h"

using namespace DATABASE;

/*#####
# PixmapTooltipTreeWidget
#####*/
PixmapTooltipTreeWidget::PixmapTooltipTreeWidget(QWidget *pParent) : QTreeWidget(pParent), m_pCurrentItem(NULL), m_TooltipSize(100, 100),
m_ToolTipPos(TOOLTIP_LEFT)
{
    setMouseTracking(true);
    this->installEventFilter(this);
    m_Timer.setSingleShot(true);
    setIndentation(0);
    connect(this, SIGNAL(itemEntered(QTreeWidgetItem*, int)), this, SLOT(_onItemEnter(QTreeWidgetItem*, int)));
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(_showPixmapTooltip()));
}

void PixmapTooltipTreeWidget::_endTracking()
{
    m_Timer.stop();
    if (m_pTooltipViewer)
        m_pTooltipViewer->hide();
    m_pCurrentItem = NULL;
}

void PixmapTooltipTreeWidget::_onItemEnter(QTreeWidgetItem *pItem, int column)
{
    if (!pItem || !showTooltip())
        return;
    if (!m_pTooltipViewer || m_pTooltipViewer->isHidden())
    {
        _endTracking();
        m_pCurrentItem = pItem;
        m_Timer.start(250);
    }
    else
    {
        m_pCurrentItem = pItem;
        _showPixmapTooltip();
    }
}

void PixmapTooltipTreeWidget::_showPixmapTooltip()
{
    if (m_pCurrentItem)
    {
        if (m_pTooltipViewer = QWidgetPtr(setTooltipWidget(m_pCurrentItem->text(0).toUInt())))
        {
            m_pTooltipViewer->setMouseTracking(false);
            m_pTooltipViewer->setWindowFlags(Qt::ToolTip);
            m_pTooltipViewer->resize(m_TooltipSize);
            QPoint widgetPos = mapToGlobal(QPoint(m_ToolTipPos == TOOLTIP_LEFT ? -m_pTooltipViewer->width() : width(), 0));
            widgetPos.setY(QCursor::pos().y());
            m_pTooltipViewer->move(widgetPos);
            m_pTooltipViewer->show();
        }
    }
}

void PixmapTooltipTreeWidget::leaveEvent(QEvent *pEvent)
{
    _endTracking();
    pEvent->accept();
}

/*#####
# AnimationPrototypeDragTreeWidget
#####*/
AnimationPrototypeDragTreeWidget::AnimationPrototypeDragTreeWidget(QWidget *pParent) : PrototypeTreeWidget(pParent)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
}

QWidget* AnimationPrototypeDragTreeWidget::setTooltipWidget(uint32 uiPrototypeID)
{
    AnimationViewDBChanger *pViewer = new AnimationViewDBChanger();
    pViewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pViewer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pViewer->setAnimationDB(m_pDB);
    pViewer->setSpriteDB(m_pSpriteDB);
    pViewer->setContinuesAnimation();
    pViewer->setCurrentAnimation(uiPrototypeID);
    pViewer->setPreviousFrameDraw(false);
    pViewer->setGridDraw(false);
    pViewer->viewport()->resize(QSize(200, 200));
    pViewer->playCurrentAnimation();
    setTooltipSize(QSize(200, 200));
    return pViewer;
}

/*#####
# TileSetPrototypeDragTreeWidget
#####*/
TileSetPrototypeDragTreeWidget::TileSetPrototypeDragTreeWidget(QWidget *pParent) : PrototypeTreeWidget(pParent)
{
    setDragEnabled(false);
    setToolTipPosition(TOOLTIP_RIGHT);
}

QWidget* TileSetPrototypeDragTreeWidget::setTooltipWidget(uint32 uiPrototypeID)
{
    ConstTileSetPrototypePtr proto;
    if (!m_pDB || !m_pDB->getItem(uiPrototypeID, proto))
        return NULL;

    QLabel *pLabel = new QLabel();
    QPixmap tileSetPixmap(TILE_SET::createTileSetPixmap(Config::Get()->getProjectDirectory(), proto, m_pTileDB));
    pLabel->setPixmap(tileSetPixmap);
    pLabel->resize(tileSetPixmap.size());
    setTooltipSize(tileSetPixmap.size());
    return pLabel;
}

void TileSetPrototypeDragTreeWidget::mousePressEvent(QMouseEvent *pEvent)
{
    PrototypeTreeWidget::mousePressEvent(pEvent);
    if (PrototypeTreeWidgetItem *pItem = dynamic_cast<PrototypeTreeWidgetItem*>(currentItem()))
        emit itemClicked(pItem->text(0).toUInt(), pEvent->button());
}


/*#####
# WorldObjectPrototypeDragTreeWidget
#####*/
WorldObjectPrototypeDragTreeWidget::WorldObjectPrototypeDragTreeWidget(QWidget *pParent) : PrototypeTreeWidget(pParent)
{
    setDragEnabled(false);
}

QWidget* WorldObjectPrototypeDragTreeWidget::setTooltipWidget(uint32 uiPrototypeID)
{
    ConstWorldObjectPrototypePtr proto;
    if (!m_pDB || !m_pDB->getItem(uiPrototypeID, proto))
        return NULL;

    AnimationViewDB *pViewer = new AnimationViewDB();
    pViewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pViewer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pViewer->setAnimationDB(m_pAnimationDB);
    pViewer->setSpriteDB(m_pSpriteDB);
    pViewer->setContinuesAnimation();
    pViewer->setCurrentAnimation(proto->getAnimationInfo(MAP::DIRECTION_DOWN).m_uiAnimationID);
    pViewer->setPreviousFrameDraw(false);
    pViewer->setGridDraw(false);
    pViewer->viewport()->resize(QSize(200, 200));
    pViewer->playCurrentAnimation();
    setTooltipSize(QSize(200, 200));
    return pViewer;
}

void WorldObjectPrototypeDragTreeWidget::leaveEvent(QEvent *pEvent)
{
    PrototypeTreeWidget::leaveEvent(pEvent);
    // create new widget cursor
    if (QTreeWidgetItem *pItem = currentItem())
    {
        // get worldobject
        ConstWorldObjectPrototypePtr objectProto;
        if (!m_pDB->getItem(pItem->text(0).toUInt(), objectProto))
            return;
        // create frame
        WorldObjectPrototype::AnimationInfo animationInfo = objectProto->getAnimationInfo(MAP::DIRECTION_DOWN);
        AnimationViewDB aniViewer;
        aniViewer.setAttribute(Qt::WA_TranslucentBackground);
        aniViewer.setWindowFlags(Qt::FramelessWindowHint);
        aniViewer.setStyleSheet("background:transparent");
        aniViewer.setFrameShape(QFrame::NoFrame);
        aniViewer.setAnimationDB(m_pAnimationDB);
        aniViewer.setSpriteDB(m_pSpriteDB);
        aniViewer.setGridDraw(false);
        aniViewer.setPreviousFrameDraw(false);
        aniViewer.setCurrentAnimation(animationInfo.m_uiAnimationID);
        // resize viewer
        AnimationViewScene *pAniScene = dynamic_cast<AnimationViewScene*>(aniViewer.scene());
        if (!pAniScene)
            return;
        QRect boundingRect = pAniScene->itemsBoundingRect().toRect();
        aniViewer.resize(boundingRect.width(), boundingRect.height());
        aniViewer.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        aniViewer.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        aniViewer.setSceneRect(boundingRect);

        // create new widget cursor
        QCursor newCursor(aniViewer.grab(), boundingRect.x(), boundingRect.y());
        emit newCursorSet(newCursor);
    }
}

/*#####
# TileDropLabel
#####*/
TileDropLabel::TileDropLabel(QWidget *pParent) : QLabel(pParent), m_uiCurrentTileID(0)
{
    setAcceptDrops(true);
}

void TileDropLabel::dragMoveEvent(QDragMoveEvent *pEvent)
{
    pEvent->accept();
}

void TileDropLabel::dragEnterEvent(QDragEnterEvent *pEvent)
{
    pEvent->acceptProposedAction();
}

void TileDropLabel::dropEvent(QDropEvent *pEvent)
{
    if (QTreeWidget *pWidget = dynamic_cast<QTreeWidget*>(pEvent->source()))
    {
        if (QTreeWidgetItem *pItem = dynamic_cast<QTreeWidgetItem*>(pWidget->currentItem()))
        {
            setCurrentTileID(pItem->text(0).toUInt());
            emit onDrop(getCurrentTileID(), Int32Point(pEvent->pos().x(), pEvent->pos().y()));
        }
    }
}

void TileDropLabel::drawCurrentTile()
{
    clear();
    if (!m_pTileDB || !m_uiCurrentTileID)
        return;

    ConstTilePrototypePtr proto;
    QPixmap pixmap;
    if (m_pTileDB->getItem(m_uiCurrentTileID, proto) && createPixmapFromTexturePrototype(Config::Get()->getProjectDirectory(), proto, pixmap))
        setPixmap(pixmap);
}