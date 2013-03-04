#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "ui/UI_MapEditor.h"
#include "ModifyObject.h"
#include "MapBrush.h"

class MapEditor : public QWidget, Ui_MapEditor
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent *pEvent) { m_ModifyObj.resizeEvent(this); }

public:
    MapEditor(QWidget *pParent = NULL);

    bool getTilePixmap(uint32 uiTileID, QPixmapPtr &result);
    void clearWidgets();

private slots:
    void _currentTileChanged(uint32 uiSpriteIndex, uint32 uiButton);
    void _updateTiles();
    void _saveMaps();
    void _loadOpenMapsFromConfig();

    void _pressBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void _releaseBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void _moveBrush(MapViewWidget *pWidget, Point3D<uint32> point);
    void _setLeftPen(bool checked);
    void _setLeftRect(bool checked);
    void _setLeftFill(bool checked);
    void _setRightPen(bool checked);
    void _setRightRect(bool checked);
    void _setRightFill(bool checked);

private:
    QPixmapPtrVector m_TileCache;
    ModifyObject m_ModifyObj;
    MAP::MapBrush m_Brush;
};
#endif