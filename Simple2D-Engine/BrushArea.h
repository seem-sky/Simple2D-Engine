#ifndef BRUSH_AREA_H
#define BRUSH_AREA_H

#include "BrushAreaInterface.h"
#include "MapTile.h"

namespace MAP
{
    class Layer;

    namespace BRUSH
    {
        namespace AREA
        {
            class Area : public Interface
            {
            private:
                virtual void _do(const MapTileInfo& info, MapTileInfoVec& tiles, PointVec<uint32>& borderTiles) = 0;

            protected:
                const MapTile& getOldMapTile() const { return m_OldTile; }
                void setOldMapTile(const MapTile& mapTile) { m_OldTile = mapTile; }

            public:
                Area(Layer& layer, const GEOMETRY::Point<uint32>& pos) : m_Layer(layer), m_BrushSize(1, 1)
                {}

                const GEOMETRY::Point<uint32>& getStartPosition() const { return m_StartPosition; }
                void setStartPosition(const GEOMETRY::Point<uint32>& pos) { m_StartPosition = pos; }

                void setBrushSize(const GEOMETRY::Point<uint32>& size);
                GEOMETRY::Point<uint32> getBrushSize() const;

                void start(MapTileInfoVec& tiles, PointVec<uint32>& borderTiles);

            private:
                GEOMETRY::Point<uint32> m_StartPosition;
                MapTile m_OldTile;
                GEOMETRY::Point<uint32> m_BrushSize;

            protected:
                const Layer& m_Layer;
            };
        }
    }
}
#endif
