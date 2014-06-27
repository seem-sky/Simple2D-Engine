#ifndef BRUSH_TYPE_H
#define BRUSH_TYPE_H

#include "MapLayer.h"
#include "BrushTypeInterface.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            class Type : public Interface
            {
            public:
                Type(uint32 tileID, Layer& layer, const UInt32Point& pos) : m_Layer(layer), m_StartPosition(pos), m_TileID(tileID)
                {}

                const UInt32Point& getStartPosition() const { return m_StartPosition; }
                uint32 getTileID() const { return m_TileID; }

                void setBorderTiles(const UInt32PointVec& tiles, REVERT::BrushRevert& revert);

            private:
                UInt32Point m_StartPosition;
                uint32 m_TileID = 0;

            protected:
                Layer& m_Layer;
            };
        }
    }
}
#endif
