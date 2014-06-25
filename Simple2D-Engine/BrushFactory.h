#ifndef BRUSH_FACTORY_H
#define BRUSH_FACTORY_H

#include "Brush.h"
#include "BrushInfo.h"

namespace MAP
{
    namespace BRUSH
    {
        class BrushFactory
        {
        public:
            static Brush2Ptr createBrush(const BrushInfo& info, Layer& mapLayer, const UInt32Point& pos = UInt32Point());
        };
    }
}
#endif
