#ifndef MAP_BRUSH_WIDGET_H
#define MAP_BRUSH_WIDGET_H

#include "MapBrush.h"
#include "UI/UI_MapBrushWidget.h"
#include "MapEditorObject.h"

class MapBrushWidget : public QWidget, public MapEditorObject, Ui_MapBrushWidget
{
    Q_OBJECT
private:
    DATABASE::MAP_STRUCTURE::MapBrushPtr _getNewTileBrush();
    DATABASE::MAP_STRUCTURE::MapBrushPtr _getNewTileSetBrush();
    DATABASE::MAP_STRUCTURE::MapBrushPtr _getNewAutoTileBrush();

public:
    MapBrushWidget(QWidget *pParent = NULL);

    void updateObject();
    void clearWidget();

    DATABASE::MAP_STRUCTURE::BRUSH::BrushMode getCurrentBrushMode() const;

    inline DATABASE::MAP_STRUCTURE::MapBrushPtr getMapBrush() { return m_pBrush; }

private:
    DATABASE::MAP_STRUCTURE::MapBrushPtr m_pBrush;

private slots:
    void _changeDrawMode(const QString& mode);

public slots:
    void changeBrush(uint32 uiID, DATABASE::MAP_STRUCTURE::BRUSH::BrushType type);
};
#endif