#ifndef BRUSH_FACTORY_H
#define BRUSH_FACTORY_H

#include "Brush.h"

namespace database
{
    class Manager;
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
            static BrushPtr createBrush(const BrushInfo& info, const database::Manager& DBMgr, LayerContainer& mapLayer, LayerType layerType, uint8 layerIndex,
                const GEOMETRY::Point<uint32>& pos = GEOMETRY::Point<uint32>());
        };
    }
}
#endif
