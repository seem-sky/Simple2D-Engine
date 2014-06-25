#ifndef TILE_MAPPING_MODE_H
#define TILE_MAPPING_MODE_H

#include "MappingModeInterface.h"
#include "Brush.h"

namespace BRUSH
{
    class MapEditorWidgetBrush;
    enum class BrushIndex;
}

namespace MAPPING_MODE
{
    class Tile : public Interface
    {
    private:
        void _finishBrush();
        bool _isTileAlreadySet(const MAP::Layer &layer, const UInt32Point& tilePos, BRUSH::BrushIndex brush);

    public:
        Tile(const BRUSH::MapEditorWidgetBrush* pBrushWidget);

        void press(MapViewer* pViewer, const QMouseEvent* pEvent);

        void release(MapViewer* pViewer, const QMouseEvent* pEvent);
        void move(MapViewer* pViewer, const QMouseEvent* pEvent);

        Type getModeType() const { return Type::TILE_MAPPING; }
        QString getModeName() const { return "tile mapping"; }

        static BRUSH::BrushIndex brushIndexFromMouseButton(Qt::MouseButton button);

    private:
        MAP::BRUSH::Brush2Ptr m_pCurrentBrush;
        MapViewer* m_pMapViewer = nullptr;
        const BRUSH::MapEditorWidgetBrush* m_pBrushWidget;
    };
}
#endif
