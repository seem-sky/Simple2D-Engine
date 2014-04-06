#include "MapViewer.h"
#include "QtGlobal.h"
#include "GraphicsTextureItem.h"
#include "AutoTileCache.h"
#include "moc_MapViewer.h"
#include <QtGui/QMouseEvent>
#include <QtCore/QTime>
#include <QtCore/QDebug>

/*#####
# MapViewerScene
#####*/
MapViewerScene::MapViewerScene(uint32 mapID, const DATABASE::DatabaseMgr& DBMgr) : QGraphicsScene(), m_MapData(DBMgr, mapID), m_ShowGrid(true),
    m_LayerType(MAP::LayerType::LAYER_BACKGROUND), m_Mode(MappingMode::TILE_MAPPING)
{
    m_LayerIndex.fill(1);
}

void MapViewerScene::setMode(MappingMode mode)
{
    if (mode == m_Mode)
        return;

    m_Mode = mode;
    update();
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
    if (m_LayerIndex.at(static_cast<uint32>(m_LayerType)) == layerIndex)
        return;
    m_LayerIndex.at(static_cast<uint32>(m_LayerType)) = layerIndex;
    update();
}

void MapViewerScene::setLayerType(MAP::LayerType layerType)
{
    if (m_LayerType == layerType)
        return;
    m_LayerType = layerType;
    update();
}

void MapViewerScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    QTime time;
    time.start();
    _drawTiles(painter, rect, MAP::LayerType::LAYER_BACKGROUND);
    qDebug() << "Background draw finishes after " << time.elapsed() << "msec.";
}

void MapViewerScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    if (getLayerType() == MAP::LayerType::LAYER_FOREGROUND || getMode() != MappingMode::TILE_MAPPING)
    {
        if (getMode() == MappingMode::TILE_MAPPING && m_MapData.getMapLayer().getLayerSize(MAP::LayerType::LAYER_FOREGROUND) == 0)
            _drawDarkRect(painter, rect);
        _drawTiles(painter, rect, MAP::LayerType::LAYER_FOREGROUND);
    }
    if (isGridActive())
        _drawGrid(painter, rect);
}

void MapViewerScene::_drawDarkRect(QPainter* painter, const QRectF& rect)
{
    painter->setOpacity(0.5);
    painter->setBrush(Qt::SolidPattern);
    auto size = m_MapData.getMapLayer().getSize();
    painter->drawRect(rect.x() < 0 ? 0 : rect.x(), rect.y() < 0 ? 0 : rect.y(),
        rect.width() > size.x*TILE_SIZE ? size.x*TILE_SIZE : rect.width(),
        rect.height() > size.y*TILE_SIZE ? size.y*TILE_SIZE : rect.height());
}

void MapViewerScene::_drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType currentLayer)
{
    const auto& mapLayer = m_MapData.getMapLayer();
    const UInt32Point startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE);
    const UInt32Point endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, mapLayer.getSize().x),
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, mapLayer.getSize().y));
    QRectF tileRect(0, 0, TILE_SIZE, TILE_SIZE);
    for (uint32 layerIndex = 0; layerIndex < mapLayer.getLayerSize(currentLayer); ++layerIndex)
    {
        switch (getMode())
        {
        case MappingMode::TILE_MAPPING:
            // draw black rect over lower layer
            if (getLayerType() == currentLayer && getLayerIndex()-1 == layerIndex)
                _drawDarkRect(painter, rect);

            // if layer above current Layer, draw opaque
            painter->setOpacity(getLayerIndex()-1 < layerIndex ? 0.4 : 1);
            break;

            // draw only foreground opaque
        case MappingMode::OBJECT_MAPPING:
            painter->setOpacity(currentLayer == MAP::LayerType::LAYER_FOREGROUND ? 0.5 : 1);
            break;
        }

        // ToDo: DrawPixmapFragments is an improvement, perhaps.
        // std::vector<std::vector<QPainter::PixmapFragment>> fragments;
        UInt32Point3D currentTile(0, 0, layerIndex);
        for (currentTile.x = startTile.x; currentTile.x < endTile.x; ++currentTile.x)
        {
            for (currentTile.y = startTile.y; currentTile.y < endTile.y; ++currentTile.y)
            {
                auto& tileObj = mapLayer.getMapTile(currentTile, currentLayer);
                if (tileObj.isEmpty())      // ignore tile ID 0; empty tile
                    continue;
                // is no auto tile
                if (tileObj.m_uiAutoTileSetID == 0)
                {
                    if (auto pPixmap = GTileCache::get()->getItem(tileObj.m_uiTileID))
                        painter->drawPixmap(currentTile.x*TILE_SIZE, currentTile.y*TILE_SIZE, TILE_SIZE, TILE_SIZE,* pPixmap);
                    //if (fragments.size() < tileObj.m_uiTileID)
                    //    fragments.resize(tileObj.m_uiTileID);
                    //fragments.at(tileObj.m_uiTileID-1).push_back(QPainter::PixmapFragment::create(QPoint(x*TILE_SIZE + TILE_SIZE/2, y*TILE_SIZE + TILE_SIZE/2),
                    //    QRect(0, 0, TILE_SIZE, TILE_SIZE)));
                }
                // autotiles
                else
                {
                    if (auto pAutoTile = GAutoTileCache::get()->getItem(tileObj.m_uiAutoTileSetID))
                    {
                        if (auto pPixmap = pAutoTile->getPixmap(static_cast<DATABASE::AUTO_TILE::AUTO_TILE_INDEX>(tileObj.m_uiTileID)))
                            painter->drawPixmap(currentTile.x*TILE_SIZE, currentTile.y*TILE_SIZE, TILE_SIZE, TILE_SIZE,* pPixmap);
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
}

void MapViewerScene::_drawGrid(QPainter* painter, const QRectF& rect)
{
    const auto& mapLayer = m_MapData.getMapLayer();
    const UInt32Point startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE*  TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE*  TILE_SIZE);
    const UInt32Point endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, mapLayer.getSize().x)*  TILE_SIZE,
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, mapLayer.getSize().y)*  TILE_SIZE);
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
MapViewer::MapViewer(uint32 mapID, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : QGraphicsView(pParent), m_DBMgr(DBMgr)
{
    setScene(new MapViewerScene(mapID, DBMgr));
    scene()->setParent(this);
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

void MapViewer::saveMap()
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        pScene->getMapData().save();
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
    return transform().m11()*  100;       // get zoom
}

void MapViewer::setMappingMode(MappingMode mode)
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        pScene->setMode(mode);
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

void MapViewer::setLayerType(MAP::LayerType layerType)
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        pScene->setLayerType(layerType);
}

MAP::LayerType MapViewer::getLayerType() const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->getLayerType();
    return MAP::LayerType::LAYER_BACKGROUND;
}

uint32 MapViewer::getMaximumLayerIndex(MAP::LayerType layerType) const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->getMapData().getMapLayer().getLayerSize(layerType);
    return 0;
}

QString MapViewer::getMapName() const
{
    if (auto pMap = m_DBMgr.getMapDatabase()->getOriginalPrototype(getMapID()))
        return pMap->getName();
    return "";
}

void MapViewer::revertLast()
{
    if (m_RevertInfos.empty())
        return;

    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
    {
        m_RevertInfos.at(m_RevertInfos.size()-1).revert(pScene->getMapData().getMapLayer());
        pScene->update();
    }
    m_RevertInfos.pop_back();
    emit changed(this);
}

void MapViewer::_finishBrush()
{
    // push only if there are changes
    if (m_pCurrentBrush && m_pCurrentBrush->getRevertInfo().hasChanges())
    {
        m_RevertInfos.push_back(m_pCurrentBrush->getRevertInfo());
        emit changed(this);
    }
    m_pCurrentBrush.reset();
}

void MapViewer::mousePressEvent(QMouseEvent* pEvent)
{
    QGraphicsView::mousePressEvent(pEvent);

    if (pEvent->button() == Qt::RightButton || pEvent->button() == Qt::LeftButton)
    {
        // create brush
        auto brush = BRUSH::BrushIndex::BRUSH_LEFT;
        if (pEvent->button() == Qt::RightButton)
            brush = BRUSH::BrushIndex::BRUSH_RIGHT;

        MAP::BRUSH::BrushInfo brushInfo;
        emit requestBrushInfo(brush, brushInfo);
        if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        {
            // if there is an old brush, release it
            if (m_pCurrentBrush)
                _finishBrush();
            m_pCurrentBrush = MAP::BRUSH::BrushFactory::createBrush(m_DBMgr, pScene->getMapData().getMapLayer(), brushInfo,
                    pScene->getLayerType(), pScene->getLayerIndex()-1);
        }

        _drawTiles(mapToScene(pEvent->pos()).toPoint());
    }
}

void MapViewer::mouseReleaseEvent(QMouseEvent* pEvent)
{
    _finishBrush();
}

void MapViewer::mouseMoveEvent(QMouseEvent* pEvent)
{
    if (!m_pCurrentBrush)
        return;

    _drawTiles(mapToScene(pEvent->pos()).toPoint());
}

void MapViewer::_drawTiles(const QPoint& pos)
{
    if (!m_pCurrentBrush)
        return;

    auto pScene = dynamic_cast<MapViewerScene*>(scene());
    if (!pScene)
        return;

    m_pCurrentBrush->draw(UInt32Point(pos.x() / TILE_SIZE, pos.y() / TILE_SIZE));
    pScene->update();
}
