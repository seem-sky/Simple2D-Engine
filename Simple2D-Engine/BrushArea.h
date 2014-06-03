#ifndef BRUSH_AREA_H
#define BRUSH_AREA_H

#include "MapLayer.h"
#include "Bitset2D.h"

namespace MAP
{
    namespace BRUSH
    {
        class BrushArea
        {
        public:
            BrushArea(const Layer &layer, const UInt32Point& center);

            virtual UInt32Point next() = NULL;
            inline UInt32Point current() const { return m_Current; }

        protected:
            UInt32Point m_Current;
            const Layer& m_MapLayer;
        };

        class BrushAreaFill : public BrushArea
        {
        public:
            BrushAreaFill(const MapTile& destTile, const Layer &layer, const UInt32Point& center);

            UInt32Point next();

        private:
            Bitset2D m_BitCheck;
            UInt32PointList m_OpenPoints;
            MapTile m_DestTile;
        };
    }
}

#endif
