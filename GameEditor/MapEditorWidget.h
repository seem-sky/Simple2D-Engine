#ifndef MAP_EDITOR_WIDGET_H
#define MAP_EDITOR_WIDGET_H

#include "ui/UI_MapEditor.h"
#include "ModifyObject.h"
#include "AutoTile.h"
#include "SharedMapEditorData.h"
#include "MapBrush.h"

class MapEditorWidget : public QWidget, Ui_MapEditor
{
    Q_OBJECT
private:
    inline void _setMappingMode(MappingMode mode) { m_pSharedData->m_MappingMode = mode; }
    void _shareData();
    void updateMapEditorWidgets();

    void _loadOpenMapsFromConfig();
    void _saveMaps();

protected:
    void resizeEvent(QResizeEvent* pEvent) { m_ModifyObj.resizeEvent(this); }

public:
    MapEditorWidget(QWidget* pParent = NULL);

    AutoTile* createAutoTilePixmaps(uint32 uiID);
    void clearWidgets();

    void setDBMgr(const DATABASE::DatabaseMgr& pDBMgr);

    void updateMapEditor();

    void storeCurrentMapInClipboard() const;

private slots:
    void _projectOpened();
    void _saveChanges();

    void _tileSetClicked(uint32 uiID, Qt::MouseButton button);
    void _autoTileClicked(uint32 uiID, Qt::MouseButton button);
    void _tileClicked(uint32 uiID, Qt::MouseButton button);

    void _pressBrush(MapViewer* pWidget, UInt32Point3D point, uint32 uiButton);
    void _releaseBrush(MapViewer* pWidget, UInt32Point3D point, uint32 uiButton);
    void _moveBrush(MapViewer* pWidget, UInt32Point3D point);

    void _layerChanged(bool state);
    void _MappingModeChanged(int index);
    void _objectTabChanged(int index);
    void _currentObjectChanged(QTreeWidgetItem* pItem, QTreeWidgetItem* pPreviousItem);

    void _zoomChanged(int zoom);
    void _layerSelected(int layer);
    void _showGridChanged(int state);
    void _revertButtonClicked(bool clicked);

    void _tabSelected(int index);

    void _mapUpdated(DATABASE::MAP_STRUCTURE::MapPrototype* map);

signals:
    void leftTileChanged(uint32 uiID, DATABASE::MAP_STRUCTURE::BRUSH::BrushType type);
    void rightTileChanged(uint32 uiID, DATABASE::MAP_STRUCTURE::BRUSH::BrushType type);

private:
    ModifyObject m_ModifyObj;
    DATABASE::MAP_STRUCTURE::BRUSH::MapObjectBrush m_MapObjectBrush;

    SharedMapEditorDataPtr m_pSharedData;
};
#endif