#include "MapViewScene.h"
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <Core/Cache/Manager.h>

MapViewScene::MapViewScene(CACHE::Manager& cacheMgr, const MAP::MAP_DATA::MapData& mapData, const database::Manager& DBMgr)
    : QGraphicsScene(), m_MapData(mapData), m_DBMgr(DBMgr), m_CacheMgr(cacheMgr)
{}

GEOMETRY::Point<uint32> MapViewScene::calculateEndTile(const QRect &rect, const GEOMETRY::Point<uint32>& startTile) const
{
    return GEOMETRY::Point<uint32>(qMin<uint32>(ceil(rect.x() % MAP::TILE_SIZE + rect.width() / MAP::TILE_SIZE) + startTile.getX() + 1,
        m_MapData.getMapLayer().getSize().getWidth()),
        qMin<uint32>(ceil(rect.y() % MAP::TILE_SIZE + rect.height() / MAP::TILE_SIZE) + startTile.getY() + 1, m_MapData.getMapLayer().getSize().getHeight()));
}

GEOMETRY::Point<uint32> MapViewScene::calculateStartTile(const QRect &rect) const
{
    return GEOMETRY::Point<uint32>(rect.x() <= 0 ? 0 : (uint32)rect.x() / MAP::TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / MAP::TILE_SIZE);
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
    class PaintInfo
    {
    public:
        void addFragment(const GEOMETRY::Point<uint32>& currentPos, const CACHE::TileCacheInfo& info)
        {
            if (!info.isValid())
                return;

            auto itr = m_Fragments.find(info.getPixmap());
            if (itr == m_Fragments.cend())
            {
                auto pair = std::make_pair(info.getPixmap(), std::vector<QPainter::PixmapFragment>());
                itr = m_Fragments.insert(pair).first;
            }

            itr->second.push_back(QPainter::PixmapFragment::create(
                QPoint(currentPos.getX() * MAP::TILE_SIZE + MAP::TILE_SIZE / 2, currentPos.getY() * MAP::TILE_SIZE + MAP::TILE_SIZE / 2),
                QRect(info.getPosition().getX(), info.getPosition().getY(), MAP::TILE_SIZE, MAP::TILE_SIZE)));
        }

        void paint(QPainter* painter) const
        {
            for (auto& fragments : m_Fragments)
                painter->drawPixmapFragments(&*fragments.second.begin(), static_cast<int>(fragments.second.size()), *fragments.first, QPainter::OpaqueHint);
        }

    private:
        std::map<const QPixmap*, std::vector<QPainter::PixmapFragment>> m_Fragments;
    };

    GEOMETRY::Point<uint32> currentTile;
    PaintInfo paintInfo;
    for (currentTile.getX() = startTile.getX(); currentTile.getX() < endTile.getX(); ++currentTile.getX())
    {
        for (currentTile.getY() = startTile.getY(); currentTile.getY() < endTile.getY(); ++currentTile.getY())
        {
            auto& tileObj = mapLayer.getMapTile(currentTile);
            if (tileObj.getMapTile().isEmpty())      // ignore tile ID 0; empty tile
                continue;
            // is no auto tile
            if (tileObj.getMapTile().m_uiAutoTileSetID == 0)
                paintInfo.addFragment(currentTile, m_CacheMgr.getTileCache().get(tileObj.getMapTile().m_uiTileID));
            // autotiles
            else
                paintInfo.addFragment(currentTile, m_CacheMgr.getAutoTileCache().get(tileObj.getMapTile().m_uiAutoTileSetID,
                static_cast<database::prototype::AutoTile::Index>(tileObj.getMapTile().m_uiTileID)));
        }
    }

    // draw fragments
    paintInfo.paint(painter);
}
