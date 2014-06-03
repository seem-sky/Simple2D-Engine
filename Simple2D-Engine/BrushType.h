#ifndef BRUSH_TYPE_H
#define BRUSH_TYPE_H

#include "MapLayer.h"
#include "BitLayer.h"

namespace MAP
{
    namespace BRUSH
    {
        class BrushType2
        {
        public:
            BrushType2(Layer& mapLayer, const MapTile& destTile);

            void setTile(const UInt32Point& at);

        private:
            Layer& m_MapLayer;
            MapTile m_DestTile;
        };
    }
}
#endif
