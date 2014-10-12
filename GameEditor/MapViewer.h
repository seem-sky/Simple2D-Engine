#ifndef MAP_VIEWER_H
#define MAP_VIEWER_H

#include <QtWidgets/QGraphicsView>
#include <Global.h>
#include <array>
#include <geometry/Point.h>
#include <Simple2D-Global.h>
#include <QtWidgets/QShortcut>

class MappingObject;
class MapViewerScene;

namespace DATABASE
{
    class DatabaseMgr;
}

namespace MAP
{
    namespace MAP_DATA
    {
        enum class MapObjectLayer;
        enum class MapDirection;
    }
}

/*#####
# MapViewer
#####*/
class MapViewer : public QGraphicsView
{
    Q_OBJECT
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
    void addWorldObject(uint32 ID, const GEOMETRY::Point<int32>& pos, MAP::MAP_DATA::MapObjectLayer layer, MAP::MAP_DATA::MapDirection direction);

private slots:
    void _onCopy();
    void _onInsert();
    void _onCutOut();

signals:
    void onCopy(MapViewerScene* pScene, QPoint pos);
    void onInsert(MapViewerScene* pScene, QPoint pos);
    void onCutOut(MapViewerScene* pScene, QPoint pos);

private:
    QShortcut* m_pActionCopy;
    QShortcut* m_pActionInsert;
    QShortcut* m_pActionCutOut;
};
#endif
