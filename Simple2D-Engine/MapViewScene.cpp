#include "MapViewScene.h"
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include "AutoTileCache.h"

MapViewScene::MapViewScene(const MAP::MAP_DATA::MapData& mapData, const DATABASE::DatabaseMgr& DBMgr) : QGraphicsScene(), m_MapData(mapData), m_DBMgr(DBMgr)
{}

GEOMETRY::Point<uint32> MapViewScene::calculateEndTile(const QRect &rect, const GEOMETRY::Point<uint32>& startTile) const
{
    return GEOMETRY::Point<uint32>(qMin<uint32>(ceil(rect.x() % TILE_SIZE + rect.width() / TILE_SIZE) + startTile.getX() + 1, m_MapData.getMapLayer().getSize().getX()),
        qMin<uint32>(ceil(rect.y() % TILE_SIZE + rect.height() / TILE_SIZE) + startTile.getY() + 1, m_MapData.getMapLayer().getSize().getY()));
}

GEOMETRY::Point<uint32> MapViewScene::calculateStartTile(const QRect &rect) const
{
    return GEOMETRY::Point<uint32>(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE);
}

void MapViewScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    QTime time;
    time.start();
    QGraphicsScene::drawBackground(painter, rect);
    drawTiles(painter, rect, MAP::LayerType::LAYER_BACKGROUND);
    qDebug() << "Background draw finishes after " << time.elapsed() << "msec.";
}

void MapViewScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    QGraphicsScene::drawForeground(painter, rect);
    drawTiles(painter, rect, MAP::LayerType::LAYER_FOREGROUND);
}

void MapViewScene::drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType currentLayer) const
{
    const GEOMETRY::Point<uint32> startTile(calculateStartTile(rect.toRect()));
    const GEOMETRY::Point<uint32> endTile(calculateEndTile(rect.toRect(), startTile));

    for (uint32 layerIndex = 0; layerIndex < m_MapData.getMapLayer().getLayerSize(currentLayer); ++layerIndex)
        drawLayer(painter, startTile, endTile, m_MapData.getMapLayer().getLayer(currentLayer, layerIndex));
}

void MapViewScene::drawLayer(QPainter* painter, const GEOMETRY::Point<uint32> startTile, const GEOMETRY::Point<uint32> endTile, const MAP::Layer& mapLayer) const
{
    // ToDo: DrawPixmapFragments is an improvement, perhaps.
    // std::vector<std::vector<QPainter::PixmapFragment>> fragments;
    GEOMETRY::Point<uint32> currentTile;
    for (currentTile.getX() = startTile.getX(); currentTile.getX() < endTile.getX(); ++currentTile.getX())
    {
        for (currentTile.getY() = startTile.getY(); currentTile.getY() < endTile.getY(); ++currentTile.getY())
        {
            auto& tileObj = mapLayer.getMapTile(currentTile);
            if (tileObj.getMapTile().isEmpty())      // ignore tile ID 0; empty tile
                continue;
            // is no auto tile
            if (tileObj.getMapTile().m_uiAutoTileSetID == 0)
            {
                if (auto pPixmap = GTileCache::get()->getItem(tileObj.getMapTile().m_uiTileID))
                    painter->drawPixmap(currentTile.getX()*TILE_SIZE, currentTile.getY()*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
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
                        painter->drawPixmap(currentTile.getX()*TILE_SIZE, currentTile.getY()*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
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
