#ifndef MAP_VIEWER_SCENE_H
#define MAP_VIEWER_SCENE_H

#include <MapViewScene.h>
#include <Global.h>
#include <array>

namespace MAP
{
    namespace BRUSH
    {
        namespace REVERT
        {
            class BrushRevert;
        }
    }
}

namespace DATABASE
{
    class DatabaseMgr;
}

class MappingObject;

/*#####
# MapViewerScene
#####*/
class MapViewerScene : public MapViewScene
{
    Q_OBJECT
private:
    void _drawGrid(QPainter* painter, const QRectF& rect) const;
    void _drawDarkRect(QPainter* painter, const QRectF& rect) const;
    //QPoint _getNearestAvailablePosition(QPoint pos, const GEOMETRY::Rectangle<int32>& boundingRect) const;

protected:
    void drawForeground(QPainter* painter, const QRectF& rect);

    void drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType currentLayer) const;

    void keyPressEvent(QKeyEvent* pKeyEvent);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * pEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * pEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent * pEvent);

public:
    MapViewerScene(uint32 mapID, const MappingObject& mappingObject, const DATABASE::DatabaseMgr& DBMgr);

    void showGrid(bool show);
    bool isGridActive() const { return m_ShowGrid; }

    void setLayerIndex(uint32 layerIndex);
    uint32 getLayerIndex() const { return m_LayerIndex.at(static_cast<uint32>(m_LayerType)); }
    void setLayerType(MAP::LayerType layerType);
    MAP::LayerType getLayerType() const { return m_LayerType; }

    const MappingObject& getMappingObject() const { return m_MappingObject; }

    // WorldObjects
    void addWorldObject(uint32 ID, QPoint pos);

    // revert
    bool hasChanged() const { return !m_Reverts.empty(); }
    void revertLast();
    void addBrushRevert(MAP::BRUSH::REVERT::BrushRevert revert);

signals:
    void onMousePress(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button);
    void onMouseRelease(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button);
    void onMouseMove(MapViewerScene* pScene, QPoint pos);
    void changed(uint32 mapID);

private:
    bool m_ShowGrid;

    std::array<uint32, 2> m_LayerIndex;
    MAP::LayerType m_LayerType;

    const MappingObject& m_MappingObject;

    std::vector<MAP::BRUSH::REVERT::BrushRevert> m_Reverts;
};
#endif
