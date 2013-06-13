#ifndef MAP_BRUSH_WIDGET_H
#define MAP_BRUSH_WIDGET_H

#include "MapBrush.h"
#include "UI/UI_MapBrushWidget.h"
#include "MapEditorObject.h"

class MapBrushWidget : public QWidget, public MapEditorObject, Ui_MapBrushWidget
{
    Q_OBJECT
private:
    MAP::MapBrushPtr _getNewTileBrush();
    MAP::MapBrushPtr _getNewAutoTileBrush();

public:
    MapBrushWidget(QWidget *pParent = NULL);

    void updateObject();
    void clearWidget();

    MAP::TileBrush::BrushType getCurrentBrushType() const;
    static MAP::TileBrush::BrushType getBrushType(const QString &mode);

    inline MAP::MapBrushPtr getMapBrush() { return m_pBrush; }

private:
    MAP::MapBrushPtr m_pBrush;

private slots:
    void _changeDrawMode(const QString& mode);

public slots:
    void changeBrush(uint32 uiID, bool isAutoTile);
};
#endif