#ifndef BRUSH_TYPE_INTERFACE_H
#define BRUSH_TYPE_INTERFACE_H

#include "MapLayer.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            class Interface
            {
            public:
                Interface(uint32 tileID, Layer& layer, const UInt32Point& pos) : m_Layer(layer), m_StartPosition(pos), m_TileID(tileID)
                {}

                const UInt32Point& getStartPosition() const { return m_StartPosition; }
                uint32 getTileID() const { return m_TileID; }

                virtual void setTiles(const MapTileInfoVec& tiles) = 0;
                virtual void setBorderTiles(const MapTileInfoVec& tiles) = 0;

            private:
                UInt32Point m_StartPosition;
                uint32 m_TileID = 0;

            protected:
                Layer& m_Layer;
            };
            typedef std::unique_ptr<Interface> TypePtr;
        }
    }
}
#endif
