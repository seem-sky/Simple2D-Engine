#ifndef MAP_VIEWER_H
#define MAP_VIEWER_H

#include <QtWidgets/QGraphicsView>
#include <Global.h>
#include <array>
#include <geometry/Point.h>
#include <Simple2D-Global.h>

class MappingObject;
class MapViewerScene;

namespace DATABASE
{
    class DatabaseMgr;
}

/*#####
# MapViewer
#####*/
class MapViewer : public QGraphicsView
{
public:
    MapViewer(uint32 mapID, const MappingObject& mappingObject, const DATABASE::DatabaseMgr& DBMgr, QWidget* pWidget = nullptr);

    bool hasChanged() const;
    void revertLast() const;
    void clearReverts() const;

    void saveMap();
    void loadMap();
    void reloadMap();

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

    // WorldObjects
    void addWorldObject(const QPoint pos, uint32 ID);
};

#endif
