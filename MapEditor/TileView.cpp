#include "TileView.h"
#include <QtGui/QBitmap>
#include <math.h>
#include <QtGui/QMouseEvent>
#include "moc_TileView.h"

TileView::TileView(QWidget* pParent) : QGraphicsView(pParent)
{
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void TileView::mousePressEvent(QMouseEvent *pEvent)
{
    if (!pEvent || (pEvent->button() != Qt::LeftButton && pEvent->button() != Qt::RightButton))
        return;

    QPoint tileCoord(pEvent->x() / m_TileSize.width(), pEvent->y() / m_TileSize.height());
    int tileID = getTile(tileCoord);
    if (tileID != -1)
    {
        emit changeTile((uint32)tileID, (uint32)pEvent->button());
        emit changeTile(tileCoord, m_TileSize, (uint32)pEvent->button());
    }
}

int TileView::getTile(QPoint at) const
{
    if (at.x() < (int32)m_uiIDVector.size() && at.y() < (int32)m_uiIDVector.at(at.x()).size())
    {
        if (m_uiIDVector.at(at.x()).at(at.y()) == MAX_UINT32)
            return -1;
        return m_uiIDVector.at(at.x()).at(at.y());
    }
    return -1;
}

void TileView::updateTiles(const QPixmapPtrMap &pixmaps, const QSize &tileSize, const QSize &parentSize)
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
    uint32 uiTilesPerRow = pixmapSize.width() / tileSize.width();
    m_uiIDVector.clear();
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
    QGraphicsScene *pScene = new QGraphicsScene();
    pScene->addPixmap(pixmap);
    setScene(pScene);
}