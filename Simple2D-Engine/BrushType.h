#ifndef BRUSH_TYPE_H
#define BRUSH_TYPE_H

#include "BrushTypeInterface.h"

namespace DATABASE
{
    class DatabaseMgr;
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
                Type(uint32 tileID, const DATABASE::DatabaseMgr& DBMgr, Layer& layer, const UInt32Point& pos);

                void setStartPosition(const UInt32Point& pos) { m_StartPosition = pos; }
                const UInt32Point& getStartPosition() const { return m_StartPosition; }
                uint32 getID() const { return m_TileID; }

                void setBorderTiles(const UInt32PointVec& tiles, REVERT::BrushRevert& revert);

            private:
                UInt32Point m_StartPosition;
                uint32 m_TileID = 0;

            protected:
                Layer& m_Layer;
                const DATABASE::DatabaseMgr& m_DBMgr;
            };
        }
    }
}
#endif
