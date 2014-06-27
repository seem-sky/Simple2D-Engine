#ifndef MAP_EDITOR_WIDGET_BRUSH_H
#define MAP_EDITOR_WIDGET_BRUSH_H

#include <QtWidgets/QWidget>
#include "MapEditorModuleBrush.h"
#include "Brush.h"

namespace MAP
{
    class Layer;
}

namespace BRUSH
{
    enum class BrushIndex
    {
        BRUSH_LEFT,
        BRUSH_RIGHT
    };

    class MapEditorWidgetBrush : public QWidget
    {
        Q_OBJECT
    private:
        void _updateOpacity();

    protected:
        void changeEvent(QEvent* pEvent);

    public:
        MapEditorWidgetBrush(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);

        const MAP::BRUSH::BrushInfo& getBrushInfo(BrushIndex brush) const;
        void setBrushInfo(BrushIndex brush, const MAP::BRUSH::BrushInfo& brushInfo);

        MAP::BRUSH::Brush2Ptr createBrush(BrushIndex brushIndex, MAP::Layer& layer) const;


    public slots:
        void onSelectionChanged(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo::Type type, uint32 ID);
        void onBrushInfoRequested(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo& brushInfo);

    private:
        std::array<MapEditorModuleBrush*, 2> m_pBrushes;
        const DATABASE::DatabaseMgr& m_DBMgr; 
    };
}

#endif
