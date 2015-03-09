#ifndef MAP_VIEWER_SCENE_H
#define MAP_VIEWER_SCENE_H

#include <MapViewScene.h>
#include <Global.h>
#include <array>
#include "WorldObjectInfo.h"

namespace CACHE
{
    class Manager;
}

class MappingObject;

struct MapEditorInfo
{
    MapEditorInfo()
    {
        m_LayerIndex.fill(0);
    }

    bool m_ShowGrid = true;

    std::array<uint8, 2> m_LayerIndex;
    MAP::LayerType m_LayerType = MAP::LayerType::LAYER_BACKGROUND;
};

/*#####
# MapViewerScene
#####*/
class MapEditorScene : public MapViewScene
{
private:
    void _drawGrid(QPainter* painter, const QRectF& rect) const;
    void _drawDarkRect(QPainter* painter, const QRectF& rect) const;
    //QPoint _getNearestAvailablePosition(QPoint pos, const GEOMETRY::Rectangle<int32>& boundingRect) const;

protected:
    void drawForeground(QPainter* painter, const QRectF& rect);
    void drawTiles(QPainter* painter, const QRectF& rect, MAP::LayerType currentLayer) const;

    void keyPressEvent(QKeyEvent *event);

public:
    MapEditorScene(CACHE::Manager& cacheMgr, const MappingObject& mappingObject, const MAP::MAP_DATA::MapData& mapData, const database::Manager& DBMgr);

    const MapEditorInfo& getMapEditorInfo() const;
    void setMapEditorInfo(const MapEditorInfo& info);

    const MappingObject& getMappingObject() const { return m_MappingObject; }

private:
    MapEditorInfo m_EditorInfo;
    const MappingObject& m_MappingObject;
};
#endif
