#ifndef TILE_MAPPING_MODE_H
#define TILE_MAPPING_MODE_H

#include "MappingModeInterface.h"
#include "Brush.h"

namespace BRUSH
{
    class MapEditorWidgetBrush;
}

namespace MAPPING_MODE
{
    class Tile : public Interface
    {
    private:
        void _finishBrush();

    public:
        Tile(const BRUSH::MapEditorWidgetBrush* pBrushWidget);

        void press(MapViewer* pViewer, const QMouseEvent* pEvent);
        void release(MapViewer* pViewer, const QMouseEvent* pEvent);
        void move(MapViewer* pViewer, const QMouseEvent* pEvent);

        Mode getModeID() const { return Mode::TILE_MAPPING; }

    private:
        MAP::BRUSH::BrushPtr m_pCurrentBrush;
        MapViewer* m_pMapViewer = nullptr;
        const BRUSH::MapEditorWidgetBrush* m_pBrushWidget;
    };
}
#endif
