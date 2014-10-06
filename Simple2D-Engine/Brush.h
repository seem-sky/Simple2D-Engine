#ifndef BRUSH_2_H
#define BRUSH_2_H

#include "BrushAreaInterface.h"
#include "BrushTypeInterface.h"
#include "BrushRevert.h"

namespace MAP
{
    namespace BRUSH
    {
        class Brush
        {
        private:
            void _checkValidation() const;

        public:
            Brush(LayerContainer& mapLayer, LayerType layerType, uint8 layerIndex, AREA::Interface* pArea = nullptr, TYPE::Interface* pType = nullptr);

            void setArea(AREA::Interface* pArea);
            void setType(TYPE::Interface* pType);

            void setPosition(const GEOMETRY::Point<uint32>& pos);

            bool hasChanges() const;
            REVERT::BrushRevert* takeBrushRevert();

            void setBrushSize(const GEOMETRY::Point<uint32>& size);

            void start();
            void start(const GEOMETRY::Point<uint32>& pos);

        private:
            AREA::AreaPtr m_pArea;
            TYPE::TypePtr m_pType;

            std::unique_ptr<REVERT::BrushRevert> m_RevertInfo;
        };
        typedef std::unique_ptr<Brush> BrushPtr;
    }
}
#endif

