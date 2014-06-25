#include "MapViewScene.h"
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include "AutoTileCache.h"

MapViewScene::MapViewScene(uint32 mapID, const DATABASE::DatabaseMgr& DBMgr) : QGraphicsScene(), m_MapData(DBMgr, mapID)
{}

UInt32Point MapViewScene::calculateEndTile(const QRect &rect, const UInt32Point& startTile) const
{
    return UInt32Point(qMin<uint32>(ceil(rect.x() % TILE_SIZE + rect.width() / TILE_SIZE) + startTile.x + 1, m_MapData.getMapLayer().getSize().x),
        qMin<uint32>(ceil(rect.y() % TILE_SIZE + rect.height() / TILE_SIZE) + startTile.y + 1, m_MapData.getMapLayer().getSize().y));
}

UInt32Point MapViewScene::calculateStartTile(const QRect &rect) const
{
    return UInt32Point(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE);
}

void MapViewScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    QTime time;
    time.start();
    drawTiles(painter, rect, MAP::LayerType::LAYER_BACKGROUND);
    qDebug() << "Background draw finishes after " << time.elapsed() << "msec.";
}

void MapViewScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    drawTiles(painter, rect, MAP::LayerType::LAYER_FOREGROUND);
}

void MapViewScene::drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType currentLayer) const
{
    const UInt32Point startTile(calculateStartTile(rect.toRect()));
    const UInt32Point endTile(calculateEndTile(rect.toRect(), startTile));

    for (uint32 layerIndex = 0; layerIndex < m_MapData.getMapLayer().getLayerSize(currentLayer); ++layerIndex)
        drawLayer(painter, startTile, endTile, m_MapData.getMapLayer().getLayer(currentLayer, layerIndex));
}

void MapViewScene::drawLayer(QPainter* painter, const UInt32Point startTile, const UInt32Point endTile, const MAP::Layer& mapLayer) const
{
    // ToDo: DrawPixmapFragments is an improvement, perhaps.
    // std::vector<std::vector<QPainter::PixmapFragment>> fragments;
    UInt32Point currentTile;
    for (currentTile.x = startTile.x; currentTile.x < endTile.x; ++currentTile.x)
    {
        for (currentTile.y = startTile.y; currentTile.y < endTile.y; ++currentTile.y)
        {
            auto& tileObj = mapLayer.getMapTile(currentTile);
            if (tileObj.getMapTile().isEmpty())      // ignore tile ID 0; empty tile
                continue;
            // is no auto tile
            if (tileObj.getMapTile().m_uiAutoTileSetID == 0)
            {
                if (auto pPixmap = GTileCache::get()->getItem(tileObj.getMapTile().m_uiTileID))
                    painter->drawPixmap(currentTile.x*TILE_SIZE, currentTile.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                //if (fragments.size() < tileObj.m_uiTileID)
                //    fragments.resize(tileObj.m_uiTileID);
                //fragments.at(tileObj.m_uiTileID-1).push_back(QPainter::PixmapFragment::create(QPoint(x*TILE_SIZE + TILE_SIZE/2, y*TILE_SIZE + TILE_SIZE/2),
                //    QRect(0, 0, TILE_SIZE, TILE_SIZE)));
            }
            // autotiles
            else
            {
                if (auto pAutoTile = GAutoTileCache::get()->getItem(tileObj.getMapTile().m_uiAutoTileSetID))
                {
                    if (auto pPixmap = pAutoTile->getPixmap(static_cast<DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_INDEX>(tileObj.getMapTile().m_uiTileID)))
                        painter->drawPixmap(currentTile.x*TILE_SIZE, currentTile.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                }
            }
        }
    }

    //// draw fragments
    //for (uint32 i = 0; i < fragments.size(); ++i)
    //{
    //    if (fragments.at(i).empty())
    //        continue;
    //    if (auto pPixmap = GTileCache::get()->getItem(i+1))
    //        painter->drawPixmapFragments(&*fragments.at(i).begin(), fragments.at(i).size(), *pPixmap, QPainter::OpaqueHint);
    //}
}
