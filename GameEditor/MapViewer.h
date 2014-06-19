#ifndef MAP_VIEWER_H
#define MAP_VIEWER_H

#include <QtWidgets/QGraphicsView>
#include "DatabaseMgr.h"
#include <QtWidgets/QGraphicsItem>
#include <MapViewScene.h>

namespace MAP
{
    namespace BRUSH
    {
        namespace REVERT
        {
            class RevertInfo;
        }
    }
}

class MappingObject;

/*#####
# MapViewerScene
#####*/
class MapViewerScene : public MapViewScene
{
private:
    void _drawGrid(QPainter* painter, const QRectF& rect) const;
    void _drawDarkRect(QPainter* painter, const QRectF& rect) const;

protected:
    void drawForeground(QPainter* painter, const QRectF& rect);

    void drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType currentLayer) const;

public:
    MapViewerScene(uint32 mapID, const DATABASE::DatabaseMgr& DBMgr);

    void showGrid(bool show);
    bool isGridActive() const { return m_ShowGrid; }

    void setLayerIndex(uint32 layerIndex);
    uint32 getLayerIndex() const { return m_LayerIndex.at(static_cast<uint32>(m_LayerType)); }
    void setLayerType(MAP::LayerType layerType);
    MAP::LayerType getLayerType() const { return m_LayerType; }

    inline void setMappingObject(MappingObject* pMappingObject) { m_pMappingObject = pMappingObject; }
    inline MappingObject* getMappingObject() const { return m_pMappingObject; }

private:
    bool m_ShowGrid;

    std::array<uint32, 2> m_LayerIndex;
    MAP::LayerType m_LayerType;

    MappingObject* m_pMappingObject = nullptr;
};

/*#####
# MapViewer
#####*/
class MapViewer : public QGraphicsView
{
    Q_OBJECT
private:
    void _drawTiles(const QPoint& pos);
    void _drawLayer() const;
    UInt32Point _calculateStartTile();

protected:
    void mousePressEvent(QMouseEvent* pEvent);
    void mouseReleaseEvent(QMouseEvent* pEvent);
    void mouseMoveEvent(QMouseEvent* pEvent);

public:
    MapViewer(uint32 mapID, const DATABASE::DatabaseMgr& DBMgr, QWidget* pWidget = nullptr);

    void saveMap();
    void loadMap();
    void updateMap();

    void setZoom(uint32 zoom);
    uint32 getZoom() const;
    void showGrid(bool show);
    bool isGridActive() const;

    void setLayerIndex(uint32 layerIndex);
    uint32 getLayerIndex() const;
    uint32 getMaximumLayerIndex(MAP::LayerType layerType) const;
    void setLayerType(MAP::LayerType layerType);
    MAP::LayerType getLayerType() const;

    uint32 getMapID() const;

    QString getMapName() const;

    void addRevertInfo(const MAP::BRUSH::REVERT::RevertInfo& info);
    void revertLast();
    inline bool hasChanges() const { return !m_RevertInfos.empty(); }

    MapViewerScene* getScene() const { return dynamic_cast<MapViewerScene*>(scene()); }

signals:
    void changed(MapViewer* pViewer);

private:
    const DATABASE::DatabaseMgr& m_DBMgr;
    std::vector<MAP::BRUSH::REVERT::RevertInfo> m_RevertInfos;
};

#endif
