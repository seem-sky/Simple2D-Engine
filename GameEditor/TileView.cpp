#include "TileView.h"
#include <QtGui/QBitmap>
#include <math.h>
#include <QtGui/QMouseEvent>
#include <QtGui/QScrollBar>
#include "moc_TileView.h"

/*#####
# TileItem
#####*/
TileItem::TileItem(ConstSharedMapEditorDataPtr pSharedData, uint32 uiID) : m_pSharedData(pSharedData), m_uiID(uiID), QGraphicsItem()
{}

void TileItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    ConstQPixmapPtr pPixmap;
    m_pSharedData->getTileCache()->getItem(m_uiID, pPixmap);
    drawItem(pPainter, pPixmap);
}

QRectF TileItem::boundingRect() const
{
    return QRectF(0, 0, TILE_SIZE+2, TILE_SIZE+2);
}

void TileItem::drawItem(QPainter *pPainter, ConstQPixmapPtr pPixmap)
{
    if (pPainter)
    {
        if (pPixmap)
            pPainter->drawPixmap(1, 1, TILE_SIZE, TILE_SIZE, *pPixmap.get(), 0, 0, TILE_SIZE, TILE_SIZE);
        pPainter->drawRect(0, 0, TILE_SIZE+1, TILE_SIZE+1);
    }
}

/*#####
# AutoTileItem
#####*/
AutoTileItem::AutoTileItem(ConstSharedMapEditorDataPtr pSharedData, uint32 uiID) : TileItem(pSharedData, uiID)
{}

void AutoTileItem::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    ConstAutoTilePtr pAutoTile;
    ConstQPixmapPtr pPixmap;
    if (m_pSharedData->getAutoTileCache()->getItem(getID(), pAutoTile))
        pAutoTile->getPixmap(DATABASE::AutoTilePrototype::INDEX_CIRCLE, pPixmap);
    drawItem(pPainter, pPixmap);
}

/*#####
# TileView
#####*/
TileView::TileView(QWidget* pParent) : QGraphicsView(pParent), MapEditorObject()
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    clearScene();
}

void TileView::updateObject()
{
    clearScene();
    if (!m_pSharedData)
        return;

    _updateWidget(frameSize().width());
}

void TileView::clearScene()
{
    if (scene())
        scene()->clear();
    else
        setScene(new QGraphicsScene());
}

void TileView::_updateWidget(int32 widgetWidth)
{
    QPoint pos;
    for (uint32 i = 0; i <= m_pSharedData->getTileCache()->getSize(); ++i)
    {
        TileItem *pItem = new TileItem(m_pSharedData, i);
        scene()->addItem(pItem);
        pItem->setPos(pos);
        pos.setX(pos.x()+pItem->boundingRect().width());
        if (pos.x()+pItem->boundingRect().width() > widgetWidth)
        {
            pos.setX(0);
            QRectF sceneRect = scene()->itemsBoundingRect();
            pos.setY(sceneRect.height() + sceneRect.y());
        }
    }
}

void TileView::mousePressEvent(QMouseEvent *pEvent)
{
    if (!scene())
        return;

    if (pEvent->button() == Qt::RightButton || pEvent->button() == Qt::LeftButton)
    {
        if (TileItem *pItem = dynamic_cast<TileItem*>(scene()->itemAt(mapToScene(pEvent->pos()))))
            emit tileClicked(pItem->getID(), pEvent->button());
    }
}

/*#####
# AutoTileView
#####*/
AutoTileView::AutoTileView(QWidget* pParent) : TileView(pParent)
{}

void AutoTileView::_updateWidget(int32 widgetWidth)
{
    QPoint pos;
    for (uint32 i = 1; i <= m_pSharedData->getAutoTileCache()->getSize(); ++i)
    {
        AutoTileItem *pItem = new AutoTileItem(m_pSharedData, i);
        scene()->addItem(pItem);
        pItem->setPos(pos);
        pos.setX(pos.x()+pItem->boundingRect().width());
        if (pos.x()+pItem->boundingRect().width() > widgetWidth)
        {
            pos.setX(0);
            QRectF sceneRect = scene()->itemsBoundingRect();
            pos.setY(sceneRect.height() + sceneRect.y());
        }
    }
}