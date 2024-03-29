#include "MapEditorScene.h"
#include "QtGlobal.h"
#include "GraphicsTextureItem.h"
#include "MappingObject.h"
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <Core/Cache/Manager.h>

/*#####
# MapViewerScene
#####*/
MapEditorScene::MapEditorScene(CACHE::Manager& cacheMgr, const MappingObject& mappingObject, const MAP::MAP_DATA::MapData& mapData, const database::Manager& DBMgr)
    : MapViewScene(cacheMgr, mapData, DBMgr), m_MappingObject(mappingObject)
{}

void MapEditorScene::drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType currentLayer) const
{
    const GEOMETRY::Point<uint32> startTile(calculateStartTile(rect.toRect()));
    const GEOMETRY::Point<uint32> endTile(calculateEndTile(rect.toRect(), startTile));

    for (uint8 layerIndex = 0; layerIndex < getMapData().getMapLayer().getLayerSize(currentLayer); ++layerIndex)
    {
        auto& mapLayer = getMapData().getMapLayer().getLayer(currentLayer, layerIndex);
        switch (m_MappingObject.getMappingModeType())
        {
        case MAPPING_MODE::Type::TILE_MAPPING:
            // draw black rect over lower layer
            if (m_EditorInfo.m_LayerType == currentLayer && m_EditorInfo.m_LayerIndex.at(static_cast<std::size_t>(currentLayer)) - 1 == layerIndex)
                _drawDarkRect(painter, rect);

            // if layer above current Layer, draw opaque
            painter->setOpacity(m_EditorInfo.m_LayerIndex.at(static_cast<uint8>(currentLayer)) - 1 < layerIndex ? 0.4 : 1);
            break;

            // draw foreground opaque
        case MAPPING_MODE::Type::OBJECT_MAPPING:
            painter->setOpacity(currentLayer == MAP::LayerType::LAYER_FOREGROUND ? 0.5 : 1);
            break;

            // draw foreground opaque
        case MAPPING_MODE::Type::SCRIPT_AREA_MAPPING:
            painter->setOpacity(currentLayer == MAP::LayerType::LAYER_FOREGROUND ? 0.5 : 1);
            break;
        }
        drawLayer(painter, startTile, endTile, getMapData().getMapLayer().getLayer(currentLayer, layerIndex));
    }
}

void MapEditorScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    QGraphicsScene::drawForeground(painter, rect);
    if (m_EditorInfo.m_LayerType == MAP::LayerType::LAYER_FOREGROUND || m_MappingObject.getMappingModeType() != MAPPING_MODE::Type::TILE_MAPPING)
    {
        if (m_MappingObject.getMappingModeType() == MAPPING_MODE::Type::TILE_MAPPING && getMapData().getMapLayer().getLayerSize(MAP::LayerType::LAYER_FOREGROUND) == 0)
            _drawDarkRect(painter, rect);
        drawTiles(painter, rect, MAP::LayerType::LAYER_FOREGROUND);
    }

    if (m_EditorInfo.m_ShowGrid)
        _drawGrid(painter, rect);
}

void MapEditorScene::_drawDarkRect(QPainter* painter, const QRectF& rect) const
{
    painter->setOpacity(0.5);
    painter->setBrush(Qt::SolidPattern);
    auto size = getMapData().getMapLayer().getSize();
    painter->drawRect(std::max(rect.x(), qreal(0)), std::max(rect.y(), qreal(0)),
        std::min(rect.width(), qreal(size.getWidth()*MAP::TILE_SIZE)),
        std::min(rect.height(), qreal(size.getHeight()*MAP::TILE_SIZE)));
}

void MapEditorScene::_drawGrid(QPainter* painter, const QRectF& rect) const
{
    const GEOMETRY::Point<uint32> startTile(calculateStartTile(rect.toRect()) * MAP::TILE_SIZE);
    const GEOMETRY::Point<uint32> endTile(calculateEndTile(rect.toRect(), startTile) * MAP::TILE_SIZE);
    QVector<QPoint> pointPairs;
    for (uint32 x = startTile.getX(); x < endTile.getX(); x += MAP::TILE_SIZE)
    {
        pointPairs.push_back(QPoint(x, startTile.getY()));
        pointPairs.push_back(QPoint(x, endTile.getY()));
    }
    for (uint32 y = startTile.getY(); y < endTile.getY(); y += MAP::TILE_SIZE)
    {
        pointPairs.push_back(QPoint(startTile.getX(), y));
        pointPairs.push_back(QPoint(endTile.getX(), y));
    }
    painter->drawLines(pointPairs);
}

void MapEditorScene::keyPressEvent(QKeyEvent* pEvent)
{
    if (pEvent->key() == Qt::Key_Escape)
        clearSelection();

    else
        MapViewScene::keyPressEvent(pEvent);
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

const MapEditorInfo& MapEditorScene::getMapEditorInfo() const
{
    return m_EditorInfo;
}

void MapEditorScene::setMapEditorInfo(const MapEditorInfo& info)
{
    m_EditorInfo = info;
    update();
}
