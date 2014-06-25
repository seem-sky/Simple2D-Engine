#ifndef BRUSH_REVERT_INFO_H
#define BRUSH_REVERT_INFO_H

#include "Simple2D-Global.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace REVERT
        {
            class BrushRevertInfo
            {
            public:
                BrushRevertInfo(LayerType type = LayerType::LAYER_BACKGROUND, uint8 index = 0) : m_LayerType(type), m_Index(index)
                {}

                LayerType getLayerType() const { return m_LayerType; }
                uint8 getLayerIndex() const { return m_Index; }

            private:
                LayerType m_LayerType;
                uint8 m_Index;
            };
        }
    }
}
#endif
