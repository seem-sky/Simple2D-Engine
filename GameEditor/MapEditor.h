#ifndef MAP_VIEWER_H
#define MAP_VIEWER_H

#include <QtWidgets/QGraphicsView>
#include <Global.h>
#include <array>
#include <geometry/Point.h>
#include <Simple2D-Global.h>
#include <QtWidgets/QShortcut>
#include "MapData.h"

namespace MAPPING_MODE
{
    enum class Type;
}

namespace MAP
{
    namespace MAP_DATA
    {
        enum class MapObjectLayer;
        enum class MapDirection;
    }

    namespace SCRIPT_AREA
    {
        class ScriptArea;
        class Data;

        namespace AREA
        {
            class Data;
        }

        namespace ACTION
        {
            class Data;
        }
    }

    namespace REVERT
    {
        class Interface;
    }
}

namespace CACHE
{
    class Manager;
}

namespace database
{
    class Manager;
    namespace prototype
    {
        class WorldObject;
    }
}

class WorldObjectItem;
class ScriptAreaItem;
class MappingObject;
class MapEditorScene;

/*#####
# MapViewer
#####*/
class MapEditor : public QGraphicsView
{
    Q_OBJECT
private:
    WorldObjectItem* _addWorldObject(const database::prototype::WorldObject* pWorldObject, MAP::MAP_DATA::WorldObjectInfo& info);
    WorldObjectItem* _setupWorldObject(const database::prototype::WorldObject* pWorldObject, WorldObjectItem* pItem, const MAP::MAP_DATA::WorldObjectInfo& info);

    ScriptAreaItem* _setupScriptArea(MAP::SCRIPT_AREA::ScriptArea* scriptArea);

    void _loadMap();
    void _setupShortcuts();

protected:
    void contextMenuEvent(QContextMenuEvent* pEvent);
    void mousePressEvent(QMouseEvent* pEvent);
    void mouseReleaseEvent(QMouseEvent* pEvent);
    void mouseMoveEvent(QMouseEvent* pEvent);
    void keyPressEvent(QKeyEvent* pEvent);
    void keyReleaseEvent(QKeyEvent* pEvent);

public:
    MapEditor(uint32 mapID, CACHE::Manager& cacheMgr, const MappingObject& mappingObject, const database::Manager& DBMgr, QWidget* pWidget = nullptr);

    void saveMap();
    void reloadMap();

    void setZoom(uint32 zoom);
    uint32 getZoom() const;
    void showGrid(bool show);
    bool isGridActive() const;
    void setLayerIndex(uint8 layerIndex);
    uint8 getLayerIndex() const;
    uint8 getMaximumLayerIndex(MAP::LayerType layerType) const;
    void setLayerType(MAP::LayerType layerType);
    MAP::LayerType getLayerType() const;

    uint32 getMapID() const;
    MAP::MAP_DATA::MapData& getMapData() { return m_MapData; }

    // WorldObjects
    WorldObjectItem* addWorldObject(uint32 ID, const GEOMETRY::Point<int32>& pos, MAP::MAP_DATA::MapObjectLayer layer, MAP::MAP_DATA::MapDirection direction);
    WorldObjectItem* addWorldObject(const MAP::MAP_DATA::WorldObjectInfo& info);
    void setWorldObject(const MAP::MAP_DATA::WorldObjectInfo& info);
    void removeWorldObject(MAP::GUID guid);
    WorldObjectItem* getWorldObject(MAP::GUID guid);

    // ScriptAreas
    ScriptAreaItem* addScriptArea(const MAP::SCRIPT_AREA::Data& data);
    ScriptAreaItem* addScriptArea(MAP::SCRIPT_AREA::ScriptArea* pScript);
    ScriptAreaItem* addScriptArea(const MAP::SCRIPT_AREA::AREA::Data& area, const MAP::SCRIPT_AREA::ACTION::Data& action);
    ScriptAreaItem* getScriptArea(MAP::GUID guid);
    ScriptAreaItem* takeScriptArea(MAP::GUID guid);
    void removeScriptArea(MAP::GUID guid);

    // revert
    void clearReverts();
    bool hasChanged() const { return !m_Reverts.empty(); }
    void revertLast();
    void addRevert(MAP::REVERT::Interface* pRevert);

private slots:
    void _onSave();
    void _onActionCopy();
    void _onActionPaste();
    void _onActionCut();
    void _onActionDelete();
    void _scriptAreaPointModified(MAP::GUID guid, uint32 index, QPoint pos);

public slots:
    void onMappingModeChanged(MAPPING_MODE::Type mode);

signals:
    void actionCopy(const MapEditor& editor);
    void actionCut(const MapEditor& editor);
    void actionPaste(MapEditor& editor, const QPoint& pos);
    void actionDelete(MapEditor& editor);

    void actionMousePress(MapEditor& editor, QMouseEvent* pEvent);
    void actionMouseRelease(MapEditor& editor, QMouseEvent* pEvent);
    void actionMouseMove(MapEditor& editor, QMouseEvent* pEvent);

    void actionKeyPress(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent);
    void actionKeyRelease(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent);

    void changed(uint32 mapID);

    void setupContextMenu(MapEditor& editor, QContextMenuEvent* pEvent);

private:
    const MappingObject& m_MappingObject;
    const database::Manager& m_DBMgr;
    MAP::MAP_DATA::MapData m_MapData;
    std::vector<std::unique_ptr<MAP::REVERT::Interface>> m_Reverts;
};
#endif
