#ifndef MAP_VIEWER_H
#define MAP_VIEWER_H

#include <QtWidgets/QGraphicsView>
#include "DatabasePrototypes.h"
#include "DatabaseMgr.h"
#include "AutoTile.h"
#include <QtWidgets/QGraphicsItem>
#include "MapData.h"
#include "MapEditorWidgetBrush.h"

enum class MappingMode
{
    TILE_MAPPING,
    OBJECT_MAPPING,
    PRESENTATION
};

/*#####
# MapViewerScene
#####*/
class MapViewerScene : public QGraphicsScene
{
private:
    void _drawGrid(QPainter* painter, const QRectF& rect);
    void _drawTiles(QPainter* painter, const QRectF& rect, MAP::Layer layer);

protected:
    void drawBackground(QPainter* painter, const QRectF& rect);
    void drawForeground(QPainter* painter, const QRectF& rect);

public:
    MapViewerScene(uint32 mapID, const DATABASE::DatabaseMgr& DBMgr);

    inline MAP::MAP_DATA::MapData& getMapData() { return m_MapData; }

    void showGrid(bool show);
    bool isGridActive() const { return m_ShowGrid; }

    void setLayerIndex(uint32 layerIndex);
    uint32 getLayerIndex() const { return m_LayerIndex.at(static_cast<uint32>(m_LayerType)); }
    void setLayerType(MAP::Layer layerType);
    MAP::Layer getLayerType() const { return m_LayerType; }

    MappingMode getMode() const { return m_Mode; }
    void setMode(MappingMode mode);

private:
    MAP::MAP_DATA::MapData m_MapData;

    bool m_ShowGrid;

    std::array<uint32, 2> m_LayerIndex;
    MAP::Layer m_LayerType;
    MappingMode m_Mode;
};

/*#####
# MapViewer
#####*/
class MapViewer : public QGraphicsView
{
    Q_OBJECT
private:
    void _drawTiles(const QPoint& pos);

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
    uint32 getMaximumLayerIndex(MAP::Layer layerType) const;
    void setLayerType(MAP::Layer layerType);
    MAP::Layer getLayerType() const;

    uint32 getMapID() const;

    QString getMapName() const;

    void revertLast();
    inline bool hasChanges() const { return !m_RevertInfos.empty(); }

    void setMappingMode(MappingMode mode);

signals:
    void requestBrushInfo(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo& brushInfo);
    void changed(MapViewer* pViewer);

private:
    const DATABASE::DatabaseMgr& m_DBMgr;

    MAP::BRUSH::BrushPtr m_pCurrentBrush;

    std::vector<MAP::BRUSH::REVERT::RevertInfo> m_RevertInfos;
};

#endif
