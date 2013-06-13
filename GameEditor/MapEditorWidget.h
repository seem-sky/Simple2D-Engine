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
    void resizeEvent(QResizeEvent *pEvent) { m_ModifyObj.resizeEvent(this); }

public:
    MapEditorWidget(QWidget *pParent = NULL);

    bool createAutoTilePixmaps(uint32 uiID, AutoTilePtr &result);
    void clearWidgets();

    void setDBMgr(const DATABASE::DatabaseMgrPtr &pDBMgr);

    void updateMapEditor();

    void storeCurrentMapInClipboard() const;

private slots:
    void _projectOpened();
    void _saveChanges();

    void _autoTileClicked(uint32 uiID, Qt::MouseButton button);
    void _tileClicked(uint32 uiID, Qt::MouseButton button);

    void _pressBrush(MapViewer *pWidget, Point3D<uint32> point, uint32 uiButton);
    void _releaseBrush(MapViewer *pWidget, Point3D<uint32> point, uint32 uiButton);
    void _moveBrush(MapViewer *pWidget, Point3D<uint32> point);

    void _layerChanged(bool state);
    void _MappingModeChanged(int index);
    void _objectTabChanged(int index);
    void _currentObjectChanged(QTreeWidgetItem *pItem, QTreeWidgetItem *pPreviousItem);

    void _zoomChanged(int zoom);
    void _layerSelected(int layer);
    void _showGridChanged(int state);
    void _revertButtonClicked(bool clicked);

    void _tabSelected(int index);

    void _mapUpdated(MAP::MapPrototypePtr map);

signals:
    void leftTileChanged(uint32 uiID, bool isAutoTile);
    void rightTileChanged(uint32 uiID, bool isAutoTile);

private:
    ModifyObject m_ModifyObj;
    MAP::MapObjectBrush m_MapObjectBrush;

    SharedMapEditorDataPtr m_pSharedData;
};
#endif