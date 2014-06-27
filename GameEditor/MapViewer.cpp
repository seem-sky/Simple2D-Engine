#include "MapViewer.h"
#include "QtGlobal.h"
#include "GraphicsTextureItem.h"
#include "moc_MapViewer.h"
#include "MappingObject.h"
#include "BrushRevert.h"
#include "DatabaseMgr.h"

/*#####
# MapViewerScene
#####*/
MapViewerScene::MapViewerScene(uint32 mapID, const DATABASE::DatabaseMgr& DBMgr) : MapViewScene(mapID, DBMgr), m_ShowGrid(true),
m_LayerType(MAP::LayerType::LAYER_BACKGROUND)
{
    m_LayerIndex.fill(1);
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

void MapViewerScene::drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType currentLayer) const
{
    const UInt32Point startTile(calculateStartTile(rect.toRect()));
    const UInt32Point endTile(calculateEndTile(rect.toRect(), startTile));

    for (uint32 layerIndex = 0; layerIndex < getMapData().getMapLayer().getLayerSize(currentLayer); ++layerIndex)
    {
        auto& mapLayer = getMapData().getMapLayer().getLayer(currentLayer, layerIndex);
        switch (m_pMappingObject->getMappingModeType())
        {
        case MAPPING_MODE::Type::TILE_MAPPING:
            // draw black rect over lower layer
            if (getLayerType() == currentLayer && getLayerIndex()-1 == layerIndex)
                _drawDarkRect(painter, rect);

            // if layer above current Layer, draw opaque
            painter->setOpacity(getLayerIndex()-1 < layerIndex ? 0.4 : 1);
            break;

            // draw only foreground opaque
        case MAPPING_MODE::Type::OBJECT_MAPPING:
            painter->setOpacity(currentLayer == MAP::LayerType::LAYER_FOREGROUND ? 0.5 : 1);
            break;
        }
        drawLayer(painter, startTile, endTile, getMapData().getMapLayer().getLayer(currentLayer, layerIndex));
    }
}

void MapViewerScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    if (getLayerType() == MAP::LayerType::LAYER_FOREGROUND || m_pMappingObject->getMappingModeType() != MAPPING_MODE::Type::TILE_MAPPING)
    {
        if (m_pMappingObject->getMappingModeType() == MAPPING_MODE::Type::TILE_MAPPING && getMapData().getMapLayer().getLayerSize(MAP::LayerType::LAYER_FOREGROUND) == 0)
            _drawDarkRect(painter, rect);
        drawTiles(painter, rect, MAP::LayerType::LAYER_FOREGROUND);
    }
    if (isGridActive())
        _drawGrid(painter, rect);
}

void MapViewerScene::_drawDarkRect(QPainter* painter, const QRectF& rect) const
{
    painter->setOpacity(0.5);
    painter->setBrush(Qt::SolidPattern);
    auto size = getMapData().getMapLayer().getSize();
    painter->drawRect(rect.x() < 0 ? 0 : rect.x(), rect.y() < 0 ? 0 : rect.y(),
        rect.width() > size.x*TILE_SIZE ? size.x*TILE_SIZE : rect.width(),
        rect.height() > size.y*TILE_SIZE ? size.y*TILE_SIZE : rect.height());
}

void MapViewerScene::_drawGrid(QPainter* painter, const QRectF& rect) const
{
    const UInt32Point startTile(calculateStartTile(rect.toRect()) * TILE_SIZE);
    const UInt32Point endTile(calculateEndTile(rect.toRect(), startTile) * TILE_SIZE);
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
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
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
    if (m_Reverts.empty())
        return;

    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
    {
        m_Reverts.back().revert(pScene->getMapData().getMapLayer());
        pScene->update();
    }
    m_Reverts.pop_back();
    emit changed(this);
}

void MapViewer::mouseReleaseEvent(QMouseEvent* pEvent)
{
    if (auto pMappingObject = getScene()->getMappingObject())
        pMappingObject->release(this, pEvent);
}

void MapViewer::mouseMoveEvent(QMouseEvent* pEvent)
{
    if (auto pMappingObject = getScene()->getMappingObject())
        pMappingObject->move(this, pEvent);
}

void MapViewer::mousePressEvent(QMouseEvent* pEvent)
{
    if (auto pMappingObject = getScene()->getMappingObject())
        pMappingObject->press(this, pEvent);
}

void MapViewer::addBrushRevert(MAP::BRUSH::REVERT::BrushRevert revert)
{
    MAP::BRUSH::REVERT::BrushRevertInfo info(getLayerType(), getLayerIndex()-1);
    revert.setBrushRevertInfo(info);
    m_Reverts.push_back(revert);
    emit changed(this);
}
