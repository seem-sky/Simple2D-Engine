#ifndef BRUSH_FACTORY_H
#define BRUSH_FACTORY_H

#include "Brush.h"

namespace DATABASE
{
    class DatabaseMgr;
}

namespace MAP
{
    class Layer;

    namespace BRUSH
    {
        class BrushInfo;

        class BrushFactory
        {
        public:
            static BrushPtr createBrush(const BrushInfo& info, const DATABASE::DatabaseMgr& DBMgr, LayerContainer& mapLayer, LayerType layerType, uint8 layerIndex,
                const GEOMETRY::Point<uint32>& pos = GEOMETRY::Point<uint32>());
        };
    }
}
#endif
