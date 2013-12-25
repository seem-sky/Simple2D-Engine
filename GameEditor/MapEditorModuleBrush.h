#ifndef MAP_EDITOR_MODULE_BRUSH_H
#define MAP_EDITOR_MODULE_BRUSH_H

#include <QtWidgets/QWidget>
#include "UI/UI_MapEditorModuleBrush.h"
#include "DatabaseMgr.h"

namespace BRUSH
{
    enum class BrushType
    {
        PEN,
        FILL
    };

    enum class SelectionType
    {
        TILES,
        AUTO_TILES,
        TILE_SETS
    };


    class MapEditorModuleBrush : public QWidget, Ui_MapEditorModuleBrush
    {
    public:
        MapEditorModuleBrush(const DATABASE::DatabaseMgr &DBMgr, QWidget *pParent = nullptr);

        void setText(const QString &text);
        void setSelection(SelectionType type, uint32 ID);

        inline BrushType getBrushType() const { return m_Type; }

    private:
        const DATABASE::DatabaseMgr &m_DBMgr;
        BrushType m_Type;
        SelectionType m_SelectionType;
        uint32 m_ID;
    };
}

#endif
