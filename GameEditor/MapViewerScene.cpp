#include "MapViewerScene.h"
#include "QtGlobal.h"
#include "GraphicsTextureItem.h"
#include "moc_MapViewerScene.h"
#include "MappingObject.h"
#include "BrushRevert.h"
#include "DatabaseMgr.h"
#include "VisualViewer.h"
#include "MapViewItem.h"
#include "DelayedDeleteObject.h"
#include <QtWidgets/QGraphicsSceneMouseEvent>

/*#####
# MapViewerScene
#####*/
MapViewerScene::MapViewerScene(uint32 mapID, const MappingObject& mappingObject, const DATABASE::DatabaseMgr& DBMgr) : MapViewScene(mapID, DBMgr), m_ShowGrid(true),
m_LayerType(MAP::LayerType::LAYER_BACKGROUND), m_MappingObject(mappingObject)
{
    m_LayerIndex.fill(1);
}

void MapViewerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * pEvent)
{
    emit onMouseRelease(this, pEvent->scenePos().toPoint(), pEvent->button());
    MapViewScene::mouseReleaseEvent(pEvent);
}

void MapViewerScene::mouseMoveEvent(QGraphicsSceneMouseEvent * pEvent)
{
    emit onMouseMove(this, pEvent->scenePos().toPoint());
    MapViewScene::mouseMoveEvent(pEvent);
}

void MapViewerScene::mousePressEvent(QGraphicsSceneMouseEvent * pEvent)
{
    emit onMousePress(this, pEvent->scenePos().toPoint(), pEvent->button());
    MapViewScene::mousePressEvent(pEvent);
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
        switch (m_MappingObject.getMappingModeType())
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
    if (getLayerType() == MAP::LayerType::LAYER_FOREGROUND || m_MappingObject.getMappingModeType() != MAPPING_MODE::Type::TILE_MAPPING)
    {
        if (m_MappingObject.getMappingModeType() == MAPPING_MODE::Type::TILE_MAPPING && getMapData().getMapLayer().getLayerSize(MAP::LayerType::LAYER_FOREGROUND) == 0)
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

MapViewItem* MapViewerScene::_addWorldObject(const DATABASE::PROTOTYPE::WORLD_OBJECT::WorldObjectPrototype* pWorldObject, MAP::MAP_DATA::WorldObjectInfo& info)
{
    // setup viewer
    auto pItem = new MapViewItem(info);
    addItem(pItem);
    return _setupWorldObject(pWorldObject, pItem, info);
}

MapViewItem* MapViewerScene::_setupWorldObject(const DATABASE::PROTOTYPE::WORLD_OBJECT::WorldObjectPrototype* pWorldObject,
    MapViewItem* pItem, const MAP::MAP_DATA::WorldObjectInfo& info)
{
    pItem->setWorldObjectBoundingRect(pWorldObject->getBoundingRect());

    VisualViewer viewer;
    viewer.setFrameShape(QFrame::NoFrame);
    viewer.showGrid(false);
    viewer.setDatabaseMgr(&getDatabaseMgr());
    auto &animationModule = pWorldObject->getAnimationModule();
    for (uint32 i = 0; i < animationModule.getAnimationCount(); ++i)
    {
        auto animationInfo = animationModule.getAnimationInfo(i);
        if (animationInfo.m_AnimationTypeID - 1 == static_cast<uint32>(info.getDirection()))
        {
            viewer.setAnimation(animationInfo);
            break;
        }
    }

    viewer.stopAnimation();
    viewer.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewer.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // set transparent background
    viewer.setStyleSheet("background:transparent;");
    viewer.setAttribute(Qt::WA_TranslucentBackground);
    viewer.setWindowFlags(Qt::FramelessWindowHint);

    auto rect = viewer.scene()->itemsBoundingRect();
    viewer.resizeToContent();
    pItem->setPixmap(viewer.grab());

    // setup position
    //pos = _getNearestAvailablePosition(QPoint(pos.x() - viewer.size().width() / 2, pos.y() - viewer.size().height() / 2), pWorldObject->getBoundingRect());
    pItem->setPos(info.getPosition().getX(), info.getPosition().getY());
    return pItem;
}

MapViewItem* MapViewerScene::addWorldObject(uint32 ID, const GEOMETRY::Point<int32>& pos, MAP::MAP_DATA::MapObjectLayer layer, MAP::MAP_DATA::MapDirection direction)
{
    if (auto pWorldObject = getDatabaseMgr().getWorldObjectDatabase()->getOriginalPrototype(ID))
    {
        // push into ObjectContainer
        auto pInfo = getMapData().getWorldObjectInfoData().addWorldObject(ID, pos, layer, direction);
        return _addWorldObject(pWorldObject, *pInfo);
    }
    return nullptr;
}

MapViewItem* MapViewerScene::addWorldObject(const MAP::MAP_DATA::WorldObjectInfo& info)
{
    if (auto pWorldObject = getDatabaseMgr().getWorldObjectDatabase()->getOriginalPrototype(info.getID()))
    {
        try
        {
            // push into ObjectContainer
            auto pInfo = getMapData().getWorldObjectInfoData().addWorldObject(info);
            return _addWorldObject(pWorldObject, *pInfo);
        }
        catch (const MAP::MAP_DATA::WorldObjectException&)
        {}
    }
    return nullptr;
}

void MapViewerScene::setWorldObject(const MAP::MAP_DATA::WorldObjectInfo& info)
{
    if (auto pProto = getDatabaseMgr().getWorldObjectDatabase()->getOriginalPrototype(info.getID()))
    {
        auto pItems = items();
        for (auto itr = pItems.begin(); itr != pItems.end(); ++itr)
        {
            if (auto pWorldObject = dynamic_cast<MapViewItem*>(*itr))
            {
                if (pWorldObject->getWorldObjectInfo().getGUID() == info.getGUID())
                    _setupWorldObject(pProto, pWorldObject, info);
            }
        }
    }
}

void MapViewerScene::removeWorldObject(MAP::MAP_DATA::GUID guid)
{
    auto pItems = items();
    for (auto itr = pItems.begin(); itr != pItems.end(); ++itr)
    {
        if (auto pWorldObject = dynamic_cast<MapViewItem*>(*itr))
        {
            if (pWorldObject->getWorldObjectInfo().getGUID() == guid)
            {
                removeItem(pWorldObject);
                getMapData().getWorldObjectInfoData().removeWorldObject(guid);
                new DelayedDeleteObject<MapViewItem>(pWorldObject);
            }
        }
    }
}

//
//namespace
//{
//    enum class Direction
//    {
//        VERTICAL,
//        HORIZONTAL
//    };
//}
//QPoint MapViewerScene::_getNearestAvailablePosition(QPoint pos, const GEOMETRY::Rectangle<int32>& boundingRect) const
//{
//    GEOMETRY::Rectangle<int32> bRect1(pos.x() + boundingRect.getLeft(), pos.y() + boundingRect.getTop(),
//        pos.x() + boundingRect.getRight(), pos.y() + boundingRect.getBottom());
//    auto pItems = items();
//    for (auto pTestItem : pItems)
//    {
//        auto resultPos = pos;
//        auto pMapItem = dynamic_cast<MapViewItem*>(pTestItem);
//        if (!pMapItem)
//            continue;
//
//        GEOMETRY::Rectangle<int32> bRect2(pMapItem->pos().x() + pMapItem->getWorldObjectBoundingRect().getLeft(),
//            pMapItem->pos().y() + pMapItem->getWorldObjectBoundingRect().getTop(),
//            pMapItem->pos().x() + pMapItem->getWorldObjectBoundingRect().getRight(),
//            pMapItem->pos().y() + pMapItem->getWorldObjectBoundingRect().getBottom());
//        auto area = bRect1.intersectionArea(bRect2);
//        if (area.isNull())
//            continue;
//
//        auto lastDirection = Direction::HORIZONTAL;
//        if (area.getWidth() <= area.getHeight())
//        {
//            // to right
//            if (bRect1.getLeft() >= bRect2.getLeft())
//            {
//                resultPos.setX(pos.x() + area.getWidth() + 1);
//            }
//            else
//            {
//                resultPos.setX(pos.x() - area.getWidth() - 1);
//            }
//        }
//        else
//        {
//            // to bottom
//            if (bRect1.getTop() >= bRect2.getTop())
//            {
//                resultPos.setY(pos.y() + area.getHeight() + 1);
//            }
//            else
//            {
//                resultPos.setY(pos.y() - area.getHeight() - 1);
//            }
//            lastDirection = Direction::VERTICAL;
//        }
//        return _getNearestAvailablePosition(resultPos, boundingRect);
//    }
//    return pos;
//}

void MapViewerScene::keyPressEvent(QKeyEvent* pKeyEvent)
{
    MapViewScene::keyPressEvent(pKeyEvent);
    emit onKeyPress(this, pKeyEvent->key());
}

void MapViewerScene::revertLast()
{
    if (!hasChanged())
        return;

    m_Reverts.back()->revert();
    update();
    m_Reverts.pop_back();
    emit changed(getMapData().getMapID());
}

void MapViewerScene::clearReverts()
{
    if (!hasChanged())
        return;

    m_Reverts.clear();
    emit changed(getMapData().getMapID());
}

void MapViewerScene::addRevert(MAP::REVERT::Interface* revert)
{
    m_Reverts.push_back(std::unique_ptr<MAP::REVERT::Interface>(revert));
    emit changed(getMapData().getMapID());
}
