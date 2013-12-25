#ifndef MAP_VIEWER_H
#define MAP_VIEWER_H

#include <QtWidgets/QGraphicsView>
#include "DatabasePrototypes.h"
#include "DatabaseMgr.h"
#include "AutoTile.h"
#include <QtWidgets/QGraphicsItem>
#include "MapData.h"

/*#####
# MapViewerScene
#####*/
class MapViewerScene : public QGraphicsScene
{
private:
    void _drawGrid(QPainter *painter, const QRectF &rect);
    void _drawTiles(QPainter *painter, const QRectF &rect, DATABASE::MAP_STRUCTURE::Layer layer);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);

public:
    MapViewerScene(uint32 mapID, const DATABASE::DatabaseMgr &DBMgr);

    inline MAP::MAP_DATA::MapData& getMapData() { return m_MapData; }

    void showGrid(bool show);
    inline bool isGridActive() const { return m_ShowGrid; }

    void setLayerIndex(uint32 layerIndex);
    inline uint32 getLayerIndex() const { return m_LayerIndex.at(m_LayerType); }
    void setLayerType(MAP::Layer layerType);
    inline MAP::Layer getLayerType() const { return m_LayerType; }

private:
    MAP::MAP_DATA::MapData m_MapData;

    bool m_ShowGrid;

    std::array<uint32, 2> m_LayerIndex;
    MAP::Layer m_LayerType;
};

/*#####
# MapViewer
#####*/
class MapViewer : public QGraphicsView
{
public:
    MapViewer(uint32 mapID, const DATABASE::DatabaseMgr &DBMgr, QWidget *pWidget = nullptr);

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

private:
    const DATABASE::DatabaseMgr &m_DBMgr;
};

#endif
