#ifndef MAP_EDITOR_WIDGET_BRUSH_H
#define MAP_EDITOR_WIDGET_BRUSH_H

#include <QtWidgets/QWidget>
#include "MapEditorModuleBrush.h"

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
        void changeEvent(QEvent *pEvent);

    public:
        MapEditorWidgetBrush(const DATABASE::DatabaseMgr &DBMgr, QWidget *pParent = nullptr);

        const MAP::BRUSH::BrushInfo& getBrushInfo(BrushIndex brush) const;
        void setBrushInfo(BrushIndex brush, const MAP::BRUSH::BrushInfo& brushInfo);

    public slots:
        void _onSelectionChanged(BRUSH::BrushIndex brush, MAP::BRUSH::SelectionType selectioType, uint32 ID);

    private:
        std::array<MapEditorModuleBrush*, 2> m_pBrushes;
        const DATABASE::DatabaseMgr &m_DBMgr; 
    };
}

#endif
