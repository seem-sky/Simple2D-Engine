#include "MapViewer.h"
#include "QtGlobal.h"
#include "GraphicsTextureItem.h"
#include "AutoTileCache.h"

/*#####
# MapViewerScene
#####*/
MapViewerScene::MapViewerScene(uint32 mapID, const DATABASE::DatabaseMgr &DBMgr) : QGraphicsScene(), m_MapData(DBMgr, mapID), m_ShowGrid(true),
    m_LayerType(MAP::LAYER_BACKGROUND)
{
}

void MapViewerScene::showGrid(bool show)
{
    if (show == m_ShowGrid)
        return;
    m_ShowGrid = show;
    update();
}

void MapViewerScene::setLayerIndex(uint32 layerIndex)
{
    if (m_LayerIndex.at(m_LayerType) == layerIndex)
        return;
    m_LayerIndex.at(m_LayerType) = layerIndex;
    update();
}

void MapViewerScene::setLayerType(MAP::Layer layerType)
{
    if (m_LayerType == layerType)
        return;
    m_LayerType = layerType;
    update();
}

void MapViewerScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    _drawTiles(painter, rect, MAP::LAYER_BACKGROUND);
}

void MapViewerScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    _drawTiles(painter, rect, MAP::LAYER_FOREGROUND);
    if (isGridActive())
        _drawGrid(painter, rect);
}

void MapViewerScene::_drawTiles(QPainter *painter, const QRectF &rect, DATABASE::MAP_STRUCTURE::Layer currentLayer)
{
    const auto &mapLayer = m_MapData.getMapLayer();
    const UInt32Point startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE);
    const UInt32Point endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, mapLayer.getSize().x),
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, mapLayer.getSize().y));
    QRectF tileRect(0, 0, TILE_SIZE, TILE_SIZE);
    for (uint32 layerIndex = 0; layerIndex < mapLayer.getLayerSize(currentLayer); ++layerIndex)
    {
        for (uint32 x = startTile.x; x < endTile.x; ++x)
        {
            for (uint32 y = startTile.y; y < endTile.y; ++y)
            {
                auto &tileObj = mapLayer.getMapTile(UInt32Point3D(x, y, layerIndex), currentLayer);
                if (tileObj.isEmpty())      // ignore tile ID 0; empty tile
                    continue;
                // is no auto tile
                if (tileObj.m_uiAutoTileSetID == 0)
                {
                    if (auto pPixmap = GTileCache::get()->getItem(tileObj.m_uiTileID))
                        painter->drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                }
                // autotiles
                else
                {
                    if (auto pAutoTile = GAutoTileCache::get()->getItem(tileObj.m_uiAutoTileSetID))
                    {
                        if (auto pPixmap = pAutoTile->getPixmap(static_cast<DATABASE::AUTO_TILE::AUTO_TILE_INDEX>(tileObj.m_uiTileID)))
                            painter->drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, *pPixmap);
                    }
                }
            }
        }
    }
}

void MapViewerScene::_drawGrid(QPainter *painter, const QRectF &rect)
{
    const auto &mapLayer = m_MapData.getMapLayer();
    const UInt32Point startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE * TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE * TILE_SIZE);
    const UInt32Point endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, mapLayer.getSize().x) * TILE_SIZE,
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, mapLayer.getSize().y) * TILE_SIZE);
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
# MapViewer
#####*/
MapViewer::MapViewer(uint32 mapID, const DATABASE::DatabaseMgr &DBMgr, QWidget *pParent) : QGraphicsView(pParent), m_DBMgr(DBMgr), m_AutoTile(2, DBMgr)
{
    setScene(new MapViewerScene(mapID, DBMgr));
    setFrameShape(QFrame::NoFrame);
}

void MapViewer::loadMap()
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
    {
        pScene->getMapData().load();
        auto mapSize = pScene->getMapData().getMapLayer().getSize();
        pScene->setSceneRect(0, 0, mapSize.x*TILE_SIZE, mapSize.y*TILE_SIZE);
        pScene->update();
    }
}

void MapViewer::updateMap()
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
    {
        pScene->getMapData().reload();
        auto mapSize = pScene->getMapData().getMapLayer().getSize();
        pScene->setSceneRect(0, 0, mapSize.x*TILE_SIZE, mapSize.y*TILE_SIZE);
        pScene->update();
    }
}

uint32 MapViewer::getMapID() const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->getMapData().getMapID();
    return 0;
}

void MapViewer::setZoom( uint32 zoom )
{
    resetTransform();
    scale(qreal(zoom)/100, qreal(zoom)/100);
}

uint32 MapViewer::getZoom() const
{
    return transform().m11() * 100;       // get zoom
}

void MapViewer::showGrid(bool show)
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        pScene->showGrid(show);
}

bool MapViewer::isGridActive() const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->isGridActive();
    return true;
}

void MapViewer::setLayerIndex(uint32 layerIndex)
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        pScene->setLayerIndex(layerIndex);
}

uint32 MapViewer::getLayerIndex() const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->getLayerIndex();
    return 0;
}

void MapViewer::setLayerType(MAP::Layer layerType)
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        pScene->setLayerType(layerType);
}

MAP::Layer MapViewer::getLayerType() const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->getLayerType();
    return MAP::LAYER_BACKGROUND;
}

uint32 MapViewer::getMaximumLayerIndex(MAP::Layer layerType) const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->getMapData().getMapLayer().getLayerSize(layerType);
    return 0;
}
