#ifndef BRUSH_AREA_H
#define BRUSH_AREA_H

#include "MapLayer.h"
#include "BrushAreaInterface.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            class Area : public Interface
            {
            private:
                virtual void _do(const MapTileInfo& info, MapTileInfoVec& tiles, UInt32PointVec& borderTiles) = 0;

            protected:
                const MapTile& getOldMapTile() const { return m_OldTile; }
                void setOldMapTile(const MapTile& mapTile) { m_OldTile = mapTile; }

            public:
                Area(Layer& layer, const UInt32Point& pos) : m_Layer(layer)
                {}

                const UInt32Point& getStartPosition() const { return m_StartPosition; }
                void setStartPosition(const UInt32Point& pos) { m_StartPosition = pos; }

                void start(MapTileInfoVec& tiles, UInt32PointVec& borderTiles);

            private:
                UInt32Point m_StartPosition;
                MapTile m_OldTile;

            protected:
                const Layer& m_Layer;
            };
        }
    }
}
#endif
