#include "ScriptArea.h"

using namespace MAP::SCRIPT_AREA;

ScriptArea::ScriptArea(MAP::GUID guid) : m_GUID(guid)
{
    if (!m_GUID)
        throw std::runtime_error("GUID can not be 0.");
}

void ScriptArea::setArea(GEOMETRY::ComplexGeometricShape<int32>* pArea)
{
    m_Area = std::unique_ptr<GEOMETRY::ComplexGeometricShape<int32>>(pArea);
}

const GEOMETRY::ComplexGeometricShape<int32>* ScriptArea::getArea() const
{
    return m_Area.get();
}

GEOMETRY::ComplexGeometricShape<int32>* ScriptArea::getArea()
{
    return m_Area.get();
}
