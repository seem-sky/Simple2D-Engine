#include "MapViewer.h"
#include "QtGlobal.h"
#include "GraphicsTextureItem.h"
#include "moc_MapViewer.h"
#include "MappingObject.h"
#include "BrushRevert.h"
#include "DatabaseMgr.h"
#include "VisualViewer.h"
#include "MapViewItem.h"
#include "DelayedDeleteObject.h"

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
    const GEOMETRY::Point<uint32> startTile(calculateStartTile(rect.toRect()));
    const GEOMETRY::Point<uint32> endTile(calculateEndTile(rect.toRect(), startTile));

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
        rect.width() > size.getX()*TILE_SIZE ? size.getX()*TILE_SIZE : rect.width(),
        rect.height() > size.getY()*TILE_SIZE ? size.getY()*TILE_SIZE : rect.height());
}

void MapViewerScene::_drawGrid(QPainter* painter, const QRectF& rect) const
{
    const GEOMETRY::Point<uint32> startTile(calculateStartTile(rect.toRect()) * TILE_SIZE);
    const GEOMETRY::Point<uint32> endTile(calculateEndTile(rect.toRect(), startTile) * TILE_SIZE);
    QVector<QPoint> pointPairs;
    for (uint32 x = startTile.getX(); x < endTile.getX(); x += TILE_SIZE)
    {
        pointPairs.push_back(QPoint(x, startTile.getY()));
        pointPairs.push_back(QPoint(x, endTile.getY()));
    }
    for (uint32 y = startTile.getY(); y < endTile.getY(); y += TILE_SIZE)
    {
        pointPairs.push_back(QPoint(startTile.getX(), y));
        pointPairs.push_back(QPoint(endTile.getX(), y));
    }
    painter->drawLines(pointPairs);
}

void MapViewerScene::addWorldObject(const QPoint pos, uint32 ID)
{
    if (auto pWorldObject = getDatabaseMgr().getWorldObjectDatabase()->getOriginalPrototype(ID))
    {
        // push into ObjectContainer
        auto pInfo = getMapData().getWorldObjectInfoData().addWorldObject(ID, GEOMETRY::Point<int32>(pos.x(), pos.y()), MAP::MAP_DATA::MapObjectLayer::MIDDLE);

        // setup viewer
        VisualViewer viewer;
        viewer.setFrameShape(QFrame::NoFrame);
        viewer.showGrid(false);
        viewer.setDatabaseManager(&getDatabaseMgr());
        viewer.setAnimation(pWorldObject->getAnimationModule().getAnimationInfo(2));
        viewer.stopAnimation();
        viewer.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        viewer.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        // set transparent background
        viewer.setStyleSheet("background:transparent;");
        viewer.setAttribute(Qt::WA_TranslucentBackground);
        viewer.setWindowFlags(Qt::FramelessWindowHint);

        auto rect = viewer.scene()->itemsBoundingRect();
        viewer.resizeToContent();

        auto pItem = new MapViewItem(*pInfo, viewer.grab());
        pItem->setWorldObjectBoundingRect(pWorldObject->getBoundingRect());
        pItem->setCenterPos(pos.x(), pos.y());
        addItem(pItem);
    }
}

void MapViewerScene::keyPressEvent(QKeyEvent* pKeyEvent)
{
    if (m_pMappingObject->getMappingModeType() != MAPPING_MODE::Type::OBJECT_MAPPING)
        return;

    auto selectedObjects = selectedItems();
    for (auto pItem : selectedObjects)
    {
        switch (pKeyEvent->key())
        {
        case Qt::Key_Delete:
            if (auto pWorldObject = dynamic_cast<MapViewItem*>(pItem))
            {
                removeItem(pWorldObject);
                getMapData().getWorldObjectInfoData().removeWorldObject(pWorldObject->getWorldObjectInfo().getGUID());
                new DelayedDeleteObject<MapViewItem>(pWorldObject);
                break;
            }

        case Qt::Key_Escape:
            pItem->setSelected(false);
            break;

        case Qt::Key_Up:
            pItem->moveBy(0, -1);
            break;
        case Qt::Key_Left:
            pItem->moveBy(-1, 0);
            break;
        case Qt::Key_Down:
            pItem->moveBy(0, 1);
            break;
        case Qt::Key_Right:
            pItem->moveBy(1, 0);
            break;
        default: break;
        }
    }
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

void MapViewer::addWorldObject(const QPoint pos, uint32 ID)
{
    if (auto pScene = getScene())
        pScene->addWorldObject(pos, ID);
}

void MapViewer::loadMap()
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
    {
        pScene->getMapData().load();
        auto mapSize = pScene->getMapData().getMapLayer().getSize();
        pScene->setSceneRect(0, 0, mapSize.getX()*TILE_SIZE, mapSize.getY()*TILE_SIZE);
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
        pScene->setSceneRect(0, 0, mapSize.getX()*TILE_SIZE, mapSize.getY()*TILE_SIZE);
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

    QGraphicsView::mouseReleaseEvent(pEvent);
}

void MapViewer::mouseMoveEvent(QMouseEvent* pEvent)
{
    if (auto pMappingObject = getScene()->getMappingObject())
        pMappingObject->move(this, pEvent);

    QGraphicsView::mouseMoveEvent(pEvent);
}

void MapViewer::mousePressEvent(QMouseEvent* pEvent)
{
    if (auto pMappingObject = getScene()->getMappingObject())
        pMappingObject->press(this, pEvent);

    QGraphicsView::mousePressEvent(pEvent);
}

void MapViewer::addBrushRevert(MAP::BRUSH::REVERT::BrushRevert revert)
{
    MAP::BRUSH::REVERT::BrushRevertInfo info(getLayerType(), getLayerIndex()-1);
    revert.setBrushRevertInfo(info);
    m_Reverts.push_back(revert);
    emit changed(this);
}
