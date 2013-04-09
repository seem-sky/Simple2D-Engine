#include "TileView.h"
#include <QtGui/QBitmap>
#include <math.h>
#include <QtGui/QMouseEvent>
#include "moc_TileView.h"

/*#####
# TileViewScene
#####*/
void TileViewScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (!parent())
        return;

    Point<uint32> tileCount = ((TileView*)parent())->getTileCount();
    const Point<uint32> startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE * TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE * TILE_SIZE);
    const Point<uint32> endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, tileCount.y) * TILE_SIZE,
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, tileCount.x) * TILE_SIZE);
    QVector<QPoint> pointPairs;
    for (uint32 x = startTile.x; x < endTile.x; x += TILE_SIZE)
    {
        pointPairs.push_back(QPoint(x, startTile.y));
        pointPairs.push_back(QPoint(x, endTile.y));
    }
    for (uint32 y = startTile.y; y < endTile.y; y += TILE_SIZE)
    {
        pointPairs.push_back(QPoint(startTile.x, y));
        pointPairs.push_back(QPoint(endTile.x, y));
    }
    painter->drawLines(pointPairs);
}

/*#####
# TileTabView
#####*/
TileTabView::TileTabView(QWidget* pParent) : QGraphicsView(pParent)
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(new TileViewScene(this));
}

Point<uint32> TileTabView::getTileCount() const
{
    if (m_uiIDVector.empty())
        return Point<uint32>();

    return Point<uint32>(m_uiIDVector.at(0).size(), m_uiIDVector.size());
}

int TileTabView::getTile(QPoint at) const
{
    if (at.x() < (int32)m_uiIDVector.size() && at.y() < (int32)m_uiIDVector.at(at.x()).size())
    {
        if (m_uiIDVector.at(at.x()).at(at.y()) == MAX_UINT32)
            return -1;
        return m_uiIDVector.at(at.x()).at(at.y());
    }
    return -1;
}

void TileTabView::updateTiles(const QPixmapPtrMap &pixmaps, const QSize &tileSize, const QSize &parentSize)
{
    if (!parent())
        return;

    m_TileSize = tileSize;
    QSize pixmapSize = parentSize;
    pixmapSize.setHeight(tileSize.width() * ceil(static_cast<float>(pixmaps.size()) / (parentSize.width() / tileSize.width())));
    QPixmap pixmap(pixmapSize);
    pixmap.fill();
    QPainter painter(&pixmap);
    QPoint tilePoint(0,0);
    m_uiIDVector.clear();
    uint32 uiTilesPerRow = pixmapSize.width() / tileSize.width();
    m_uiIDVector.resize(uiTilesPerRow, UInt32Vector(ceil((float)pixmaps.size() / uiTilesPerRow), MAX_UINT32));
    for (QPixmapPtrMap::const_iterator itr = pixmaps.begin(); itr != pixmaps.end(); ++itr, tilePoint.setX(tilePoint.x() + tileSize.width()))
    {
        if (!itr->second)
            continue;

        if (tilePoint.x() + tileSize.width() > parentSize.width())
        {
            tilePoint.setY(tilePoint.y() + tileSize.height());
            tilePoint.setX(0);
        }

        painter.drawTiledPixmap(tilePoint.x(), tilePoint.y(), tileSize.width(), tileSize.height(), *itr->second.get());
        m_uiIDVector.at(tilePoint.x() / tileSize.width()).at(tilePoint.y() / tileSize.height()) = itr->first;
    }

    painter.end();
    QGraphicsScene *pScene = new TileViewScene(this);
    pScene->addPixmap(pixmap);
    setScene(pScene);
}

/*#####
# TileView
#####*/
void TileView::mousePressEvent(QMouseEvent *pEvent)
{
    if (!pEvent || (pEvent->button() != Qt::LeftButton && pEvent->button() != Qt::RightButton))
        return;

    QPoint scenePos = mapToScene(pEvent->pos()).toPoint();
    QPoint tileCoord(scenePos.x() / getTileSize().width(), scenePos.y() / getTileSize().height());
    int tileID = getTile(tileCoord);
    if (tileID != -1)
    {
        emit changeTile((uint32)tileID, (uint32)pEvent->button(), isAutoTileView());
        emit changeTile(tileCoord, getTileSize(), (uint32)pEvent->button());
    }
}