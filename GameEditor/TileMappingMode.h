#ifndef TILE_MAPPING_MODE_H
#define TILE_MAPPING_MODE_H

#include "MappingModeInterface.h"
#include "Brush.h"
#include "BrushInfo.h"
#include <array>

namespace BRUSH
{
    enum class BrushIndex;
}

namespace MAP
{
    class Layer;
}

namespace DATABASE
{
    class DatabaseMgr;
}

namespace MAPPING_MODE
{
    class Tile : public Interface
    {
        Q_OBJECT
    private:
        void _finishBrush();
        bool _isTileAlreadySet(const MAP::Layer &layer, const GEOMETRY::Point<uint32>& tilePos, const MAP::BRUSH::BrushInfo& brushInfo);

    public:
        Tile(const DATABASE::DatabaseMgr& DBMgr, QObject* pParent = nullptr);

        void press(MapEditor& editor, const QPoint& pos, Qt::MouseButton button);
        void release(MapEditor& editor, const QPoint& pos, Qt::MouseButton button);
        void move(MapEditor& editor, const QPoint& pos);

        void keyPress(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent) {}
        void keyRelease(MapEditor& editor, const QPoint& pos, QKeyEvent* pEvent) {}

        void copy(const MapEditor& editor) {}
        void cut(const MapEditor& editor) {}
        void paste(MapEditor& editor, const QPoint& pos) {}
        void remove(MapEditor& editor) {}

        Type getModeType() const { return Type::TILE_MAPPING; }
        QString getModeName() const { return "tile mapping"; }

        static BRUSH::BrushIndex brushIndexFromMouseButton(Qt::MouseButton button);

    public slots:
        void onBrushInfoChanged(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo info);

    private:
        const DATABASE::DatabaseMgr& m_DBMgr;

        MAP::BRUSH::BrushPtr m_pCurrentBrush;
        MapEditor* m_pCurrentEditor = nullptr;

        std::array<MAP::BRUSH::BrushInfo, 2> m_BrushInfos;
    };
}
#endif
