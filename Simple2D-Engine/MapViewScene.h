#ifndef MAP_VIEW_SCENE_H
#define MAP_VIEW_SCENE_H

#include "MapData.h"
#include <QtWidgets/QGraphicsScene>

class MapViewScene : public QGraphicsScene
{
protected:
    virtual void drawBackground(QPainter* painter, const QRectF& rect);
    virtual void drawForeground(QPainter* painter, const QRectF& rect);

    virtual void drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType layer) const;
    void drawLayer(QPainter* painter, const GEOMETRY::Point<uint32> startTile, const GEOMETRY::Point<uint32> endTile, const MAP::Layer& mapLayer) const;

    GEOMETRY::Point<uint32> calculateStartTile(const QRect& rect) const;
    GEOMETRY::Point<uint32> calculateEndTile(const QRect& rect, const GEOMETRY::Point<uint32>& startTile) const;

public:
    MapViewScene(uint32 mapID, const DATABASE::DatabaseMgr& DBMgr);

    inline MAP::MAP_DATA::MapData& getMapData() { return m_MapData; }
    inline const MAP::MAP_DATA::MapData& getMapData() const { return m_MapData; }

    const DATABASE::DatabaseMgr& getDatabaseMgr() const { return m_DBMgr; }

private:
    MAP::MAP_DATA::MapData m_MapData;
    const DATABASE::DatabaseMgr& m_DBMgr;
};
#endif