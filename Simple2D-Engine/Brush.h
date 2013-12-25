#ifndef BRUSH_H
#define BRUSH_H

#include "MapLayer.h"

namespace MAP
{
    namespace BRUSH
    {
        enum class BrushType
        {
            PEN,
            FILL
        };

        enum class SelectionType
        {
            TILES,
            AUTO_TILES,
            TILE_SETS
        };

        class BrushInfo
        {
        public:
            BrushInfo() : m_Type(BrushType::PEN), m_SelectionType(SelectionType::TILES), m_ID(0)
            {
            }

            BrushType m_Type;
            SelectionType m_SelectionType;
            uint32 m_ID;
        };

        class Brush
        {
        public:
            Brush(MapLayer& mapLayer);

            void setBrushInfo(const BrushInfo& brushInfo);
            inline const BrushInfo& getBrushInfo() const { return m_BrushInfo; }

        private:
            MapLayer& m_MapLayer;
            BrushInfo m_BrushInfo;
        };
    }
}

#endif
