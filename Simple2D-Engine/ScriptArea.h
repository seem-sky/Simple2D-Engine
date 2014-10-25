#ifndef SCRIPT_AREA_H
#define SCRIPT_AREA_H

#include "ScriptArea_AreaInterface.h"
#include <memory>
#include <Simple2D-Global.h>
#include <geometry/ComplexGeometricShape.h>
#include <Typedefs.h>

namespace MAP
{
    namespace SCRIPT_AREA
    {
        const uint32 MINIMUM_SIZE = TILE_SIZE;
        class ScriptArea
        {
        public:
            ScriptArea(MAP::GUID guid);

            void setArea(GEOMETRY::ComplexGeometricShape<int32>* pArea);
            GEOMETRY::ComplexGeometricShape<int32>* getArea();
            const GEOMETRY::ComplexGeometricShape<int32>* getArea() const;

            GUID getGUID() const { return m_GUID; }

        private:
            std::unique_ptr<GEOMETRY::ComplexGeometricShape<int32>> m_Area;
            GUID m_GUID = 0;
        };
    }
}
#endif
