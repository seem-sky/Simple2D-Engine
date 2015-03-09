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

namespace database
{
    class Manager;
}

namespace CACHE
{
    class Manager;
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
        MapEditorWidgetBrush(CACHE::Manager& cacheMgr, const database::Manager& DBMgr, QWidget* pParent = nullptr);

        void clear();

    public slots:
        void onSelectionChanged(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo::Type type, uint32 ID);
        void onBrushChanged(const MapEditorModuleBrush* pModule);

    signals:
        void changeBrushInfo(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo info);

    private:
        std::array<MapEditorModuleBrush*, 2> m_pBrushes;
        const database::Manager& m_DBMgr; 
    };
}
#endif
