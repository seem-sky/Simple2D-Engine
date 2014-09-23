#include "MapViewer.h"
#include "QtGlobal.h"
#include "MappingObject.h"
#include "DatabaseMgr.h"
#include "MapViewerScene.h"
#include "BrushRevert.h"

/*#####
# MapViewer
#####*/
MapViewer::MapViewer(uint32 mapID, const MappingObject& mappingObject, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : QGraphicsView(pParent)
{
    setScene(new MapViewerScene(mapID, mappingObject, DBMgr));
    scene()->setParent(this);
    setFrameShape(QFrame::NoFrame);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void MapViewer::addWorldObject(const QPoint pos, uint32 ID)
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        pScene->addWorldObject(ID, pos);
}

bool MapViewer::hasChanged() const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->hasChanged();
    return false;
}

void MapViewer::revertLast() const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->revertLast();
}

void MapViewer::clearReverts() const
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        return pScene->clearReverts();
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
    {
        pScene->getMapData().save();
        pScene->clearReverts();
    }
}

void MapViewer::reloadMap()
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
