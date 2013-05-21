#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "ui/UI_MapEditor.h"
#include "ModifyObject.h"
#include "AutoTilePixmaps.h"

class MapEditor : public QWidget, Ui_MapEditor
{
    Q_OBJECT
public:
    enum MappingMode
    {
        MODE_TILE,
        MODE_OBJECT
    };

private:
    MAP::MapBrushPtr& _updateNewPoint(const MAP::MapBrushPtr &pOldBrush, MAP::MapBrushPtr &pNewBrush);

    inline void _setMappingMode(MappingMode mode) { m_MappingMode = mode; }

    MAP::TileBrush::BrushType _getBrushType(const QString &sMode);

protected:
    void resizeEvent(QResizeEvent *pEvent) { m_ModifyObj.resizeEvent(this); }

public:
    MapEditor(QWidget *pParent = NULL);

    bool getTilePixmap(uint32 uiTileID, QPixmapPtr &result) const;
    bool createAutoTilePixmaps(uint32 uiID, AutoTilePixmapsPtr &result);
    bool getAutoTilePixmaps(uint32 uiID, AutoTilePixmapsPtr &result) const;
    void clearWidgets();
    
    inline DATABASE::DatabaseMgrPtr getDBMgr() const { return m_pDBMgr; }
    void setDBMgr(const DATABASE::DatabaseMgrPtr &pDBMgr);

    void updateMapEditor();

private slots:
    void _currentTileChanged(uint32 uiSpriteIndex, uint32 uiButton, bool autoTile);
    void _updateTiles();
    void _saveMaps();
    void _loadOpenMapsFromConfig();

    void _pressBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void _releaseBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void _moveBrush(MapViewWidget *pWidget, Point3D<uint32> point);
    void _LHDrawModeChanged(const QString &sMode);
    void _RHDrawModeChanged(const QString &sMode);

    void _MappingModeChanged(int index);
    void _objectTabChanged(int index);
    void _currentObjectChanged(QTreeWidgetItem *pItem, QTreeWidgetItem *pPreviousItem);

private:
    AutoTilePixmapsPtrVector m_AutoTilePixmaps;
    QPixmapPtrVector m_TileCache;
    ModifyObject m_ModifyObj;
    MAP::MapBrushPtr m_pLHBrush;
    MAP::MapBrushPtr m_pRHBrush;
    MAP::MapObjectBrush m_MapObjectBrush;
    DATABASE::DatabaseMgrPtr m_pDBMgr;

    MappingMode m_MappingMode;
};
#endif