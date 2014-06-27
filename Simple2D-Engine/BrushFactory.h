#ifndef BRUSH_FACTORY_H
#define BRUSH_FACTORY_H

#include "Brush.h"

namespace MAP
{
    class Layer;

    namespace BRUSH
    {
        class BrushInfo;

        class BrushFactory
        {
        public:
            static Brush2Ptr createBrush(const BrushInfo& info, Layer& mapLayer, const UInt32Point& pos = UInt32Point());
        };
    }
}
#endif
