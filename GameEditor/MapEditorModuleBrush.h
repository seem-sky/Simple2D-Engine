#ifndef MAP_EDITOR_MODULE_BRUSH_H
#define MAP_EDITOR_MODULE_BRUSH_H

#include <QtWidgets/QWidget>
#include "UI/UI_MapEditorModuleBrush.h"
#include "DatabaseMgr.h"
#include <BrushInfo.h>

namespace CACHE
{
    class Manager;
}

namespace BRUSH
{
    class MapEditorModuleBrush : public QWidget, Ui_MapEditorModuleBrush
    {
    Q_OBJECT
        void _update();

    public:
        MapEditorModuleBrush(CACHE::Manager& cacheMgr, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);

        void clear();
        void setText(const QString& text);

        void setBrushInfo(const MAP::BRUSH::BrushInfo& brushInfo);
        inline const MAP::BRUSH::BrushInfo& getBrushInfo() const { return m_BrushInfo; }

    private slots:
        void _onBrushTypeChanged(const QString& text);

    signals:
        void changeBrush(const MapEditorModuleBrush* module);

    private:
        const DATABASE::DatabaseMgr& m_DBMgr;
        CACHE::Manager& m_CacheMgr;
        MAP::BRUSH::BrushInfo m_BrushInfo;
    };
}

#endif
