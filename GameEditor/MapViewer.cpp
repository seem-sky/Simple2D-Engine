#include "MapViewer.h"
#include "moc_MapViewer.h"
#include "QtGlobal.h"
#include "MappingObject.h"
#include "DatabaseMgr.h"
#include "MapViewerScene.h"
#include "BrushRevert.h"
#include "WorldObjectInfo.h"
#include <QtGui/QCursor>

/*#####
# MapViewer
#####*/
MapViewer::MapViewer(uint32 mapID, const MappingObject& mappingObject, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : QGraphicsView(pParent),
m_pActionCopy(new QShortcut(QKeySequence(tr("Ctrl+C", "Action|Copy")), this)), m_pActionInsert(new QShortcut(QKeySequence(tr("Ctrl+V", "Action|Insert")), this)),
m_pActionCutOut(new QShortcut(QKeySequence(tr("Ctrl+X", "Action|CutOut")), this))
{
    setScene(new MapViewerScene(mapID, mappingObject, DBMgr));
    scene()->setParent(this);
    setFrameShape(QFrame::NoFrame);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    connect(m_pActionCopy, SIGNAL(activated()), this, SLOT(_onCopy()));
    connect(m_pActionInsert, SIGNAL(activated()), this, SLOT(_onInsert()));
    connect(m_pActionCutOut, SIGNAL(activated()), this, SLOT(_onCutOut()));
}

void MapViewer::addWorldObject(uint32 ID, const GEOMETRY::Point<int32>& pos, MAP::MAP_DATA::MapObjectLayer layer, MAP::MAP_DATA::MapDirection direction)
{
    if (auto pScene = dynamic_cast<MapViewerScene*>(scene()))
        pScene->addWorldObject(ID, pos, layer, direction);
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

void MapViewer::_onCopy()
{
    auto posWidget(mapFromGlobal(QCursor::pos()));
    if (rect().contains(posWidget))
        emit onCopy(dynamic_cast<MapViewerScene*>(scene()), mapToScene(posWidget).toPoint());
}

void MapViewer::_onInsert()
{
    auto posWidget(mapFromGlobal(QCursor::pos()));
    if (rect().contains(posWidget))
        emit onInsert(dynamic_cast<MapViewerScene*>(scene()), mapToScene(posWidget).toPoint());
}

void MapViewer::_onCutOut()
{
    auto posWidget(mapFromGlobal(QCursor::pos()));
    if (rect().contains(posWidget))
        emit onCutOut(dynamic_cast<MapViewerScene*>(scene()), mapToScene(posWidget).toPoint());
}
