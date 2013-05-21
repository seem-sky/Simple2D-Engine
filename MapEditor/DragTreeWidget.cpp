#include "DragTreeWidget.h"
#include "QtGlobal.h"
#include "AnimationDatabaseWidget.h"
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include "moc_DragTreeWidget.h"

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

void PixmapTooltipTreeWidget::_onItemEnter(QTreeWidgetItem *pItem, int column)
{
    if (!pItem || !showTooltip())
        return;
    if (!m_pTooltipViewer || m_pTooltipViewer->isHidden())
        _beginTracking(pItem);
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

void PixmapTooltipTreeWidget::_endTracking()
{
    m_Timer.stop();
    if (m_pTooltipViewer)
        m_pTooltipViewer->hide();
    m_pCurrentItem = NULL;
}

void PixmapTooltipTreeWidget::_beginTracking(QTreeWidgetItem* pItem)
{
    _endTracking();
    m_pCurrentItem = pItem;
    m_Timer.start(250);
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
# WorldObjectPrototypeDragTreeWidget
#####*/
WorldObjectPrototypeDragTreeWidget::WorldObjectPrototypeDragTreeWidget(QWidget *pParent) : PrototypeTreeWidget(pParent)
{
    setDragEnabled(false);
    //setDragDropMode(QAbstractItemView::DragOnly);
}

QWidget* WorldObjectPrototypeDragTreeWidget::setTooltipWidget(uint32 uiPrototypeID)
{
    ConstWorldObjectPrototypePtr proto;
    if (!m_pDB || !m_pDB->getPrototype(uiPrototypeID, proto))
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
        if (!m_pDB->getPrototype(pItem->text(0).toUInt(), objectProto))
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
        QCursor newCursor(QPixmap::grabWidget(&aniViewer), boundingRect.x(), boundingRect.y());
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
            emit onDrop(getCurrentTileID(), Point<int32>(pEvent->pos().x(), pEvent->pos().y()));
        }
    }
}

void TileDropLabel::drawCurrentTile()
{
    if (!m_pTileDB || !m_uiCurrentTileID)
        return;

    ConstTilePrototypePtr proto;
    QPixmap pixmap;
    if (m_pTileDB->getPrototype(m_uiCurrentTileID, proto) && createPixmapFromTexturePrototype(proto, pixmap))
        setPixmap(pixmap);
}