#ifndef MAP_EDITOR_WIDGET_BRUSH_H
#define MAP_EDITOR_WIDGET_BRUSH_H

#include "ToolWidget.h"
#include <Brush.h>
#include <BrushInfo.h>
#include <array>

namespace MAP
{
    class Layer;
}

namespace DATABASE
{
    class DatabaseMgr;
}

namespace BRUSH
{
    class MapEditorModuleBrush;

    enum class BrushIndex
    {
        BRUSH_LEFT,
        BRUSH_RIGHT
    };

    class MapEditorWidgetBrush : public ToolWidget
    {
        Q_OBJECT
    public:
        MapEditorWidgetBrush(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);

        void clear();

    public slots:
        void onSelectionChanged(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo::Type type, uint32 ID);
        void onBrushChanged(const MapEditorModuleBrush* pModule);

    signals:
        void changeBrushInfo(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo info);

    private:
        std::array<MapEditorModuleBrush*, 2> m_pBrushes;
        const DATABASE::DatabaseMgr& m_DBMgr; 
    };
}

#endif
