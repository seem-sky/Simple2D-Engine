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

        void press(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button);
        void release(MapViewerScene* pScene, QPoint pos, Qt::MouseButton button);
        void move(MapViewerScene* pScene, QPoint pos);
        void copy(MapViewerScene* pScene, QPoint pos);
        void insert(MapViewerScene* pScene, QPoint pos);
        void cutOut(MapViewerScene* pScene, QPoint pos);

        Type getModeType() const { return Type::TILE_MAPPING; }
        QString getModeName() const { return "tile mapping"; }

        static BRUSH::BrushIndex brushIndexFromMouseButton(Qt::MouseButton button);

    public slots:
        void onBrushInfoChanged(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo info);

    private:
        const DATABASE::DatabaseMgr& m_DBMgr;

        MAP::BRUSH::BrushPtr m_pCurrentBrush;
        MapViewerScene* m_pCurrentScene = nullptr;

        std::array<MAP::BRUSH::BrushInfo, 2> m_BrushInfos;
    };
}
#endif