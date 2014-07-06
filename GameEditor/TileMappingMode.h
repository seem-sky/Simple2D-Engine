#ifndef TILE_MAPPING_MODE_H
#define TILE_MAPPING_MODE_H

#include "MappingModeInterface.h"
#include "Brush.h"

namespace BRUSH
{
    class MapEditorWidgetBrush;
    enum class BrushIndex;
}

namespace MAP
{
    class Layer;

    namespace BRUSH
    {
        class BrushInfo;
    }
}

namespace MAPPING_MODE
{
    class Tile : public Interface
    {
    private:
        void _finishBrush();
        bool _isTileAlreadySet(const MAP::Layer &layer, const UInt32Point& tilePos, const MAP::BRUSH::BrushInfo& brushInfo);

    public:
        Tile(const BRUSH::MapEditorWidgetBrush& brushWidget);

        void press(MapViewer* pViewer, const QMouseEvent* pEvent);

        void release(MapViewer* pViewer, const QMouseEvent* pEvent);
        void move(MapViewer* pViewer, const QMouseEvent* pEvent);

        Type getModeType() const { return Type::TILE_MAPPING; }
        QString getModeName() const { return "tile mapping"; }

        static BRUSH::BrushIndex brushIndexFromMouseButton(Qt::MouseButton button);

    private:
        MAP::BRUSH::BrushPtr m_pCurrentBrush;
        MapViewer* m_pMapViewer = nullptr;
        const BRUSH::MapEditorWidgetBrush& m_BrushWidget;
    };
}
#endif
