#ifndef MAP_BRUSH_WIDGET_H
#define MAP_BRUSH_WIDGET_H

#include "MapBrush.h"
#include "UI/UI_MapBrushWidget.h"
#include "MapEditorObject.h"

class MapBrushWidget : public QWidget, public MapEditorObject, Ui_MapBrushWidget
{
    Q_OBJECT
private:
    DATABASE::MAP::MapBrushPtr _getNewTileBrush();
    DATABASE::MAP::MapBrushPtr _getNewTileSetBrush();
    DATABASE::MAP::MapBrushPtr _getNewAutoTileBrush();

public:
    MapBrushWidget(QWidget *pParent = NULL);

    void updateObject();
    void clearWidget();

    DATABASE::MAP::BRUSH::BrushMode getCurrentBrushMode() const;

    inline DATABASE::MAP::MapBrushPtr getMapBrush() { return m_pBrush; }

private:
    DATABASE::MAP::MapBrushPtr m_pBrush;

private slots:
    void _changeDrawMode(const QString& mode);

public slots:
    void changeBrush(uint32 uiID, DATABASE::MAP::BRUSH::BrushType type);
};
#endif