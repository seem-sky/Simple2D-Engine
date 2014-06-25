#ifndef BRUSH_AREA_INTERFACE_H
#define BRUSH_AREA_INTERFACE_H

#include "MapLayer.h"
#include <Bitset2D.h>

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            class Interface
            {
            public:
                Interface(Layer& layer, const UInt32Point& pos) : m_Layer(layer)
                {}

                const UInt32Point& getStartPosition() const { return m_StartPosition; }
                void setStartPosition(const UInt32Point& pos) { m_StartPosition = pos; }

                virtual void start(MapTileInfoVec& tiles, MapTileInfoVec& borderTiles) = 0;

            private:
                UInt32Point m_StartPosition;

            protected:
                const Layer& m_Layer;
            };
            typedef std::unique_ptr<Interface> AreaPtr;
        }
    }
}
#endif
