#ifndef BRUSH_TYPE_H
#define BRUSH_TYPE_H

#include "BrushTypeInterface.h"

namespace database
{
    class Manager;
}

namespace MAP
{
    class Layer;

    namespace BRUSH
    {
        namespace TYPE
        {
            class Type : public Interface
            {
            public:
                Type(uint32 tileID, const database::Manager& DBMgr, Layer& layer, const GEOMETRY::Point<uint32>& pos);

                void setStartPosition(const GEOMETRY::Point<uint32>& pos) { m_StartPosition = pos; }
                const GEOMETRY::Point<uint32>& getStartPosition() const { return m_StartPosition; }
                uint32 getID() const { return m_TileID; }

                void setBorderTiles(const PointVec<uint32>& tiles, REVERT::BrushRevert& revert);

            private:
                GEOMETRY::Point<uint32> m_StartPosition;
                uint32 m_TileID = 0;

            protected:
                Layer& m_Layer;
                const database::Manager& m_DBMgr;
            };
        }
    }
}
#endif
