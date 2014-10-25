#ifndef SCRIPT_AREA_AREA_BASE_H
#define SCRIPT_AREA_AREA_BASE_H

#include "ScriptArea_AreaInterface.h"
#include <geometry/algorithm.h>
#include <geometry/Triangle.h>

namespace MAP
{
    namespace SCRIPT_AREA
    {
        namespace AREA
        {
            template <class T>
            class AreaBase : public Interface
            {
            protected:
                void setupTriangles()
                {
                    m_Triangles.clear();
                    try
                    {
                        m_Triangles = GEOMETRY::triangulate(m_Area);
                    }
                    catch (const std::runtime_error&) {}
                }

            public:
                AreaBase(const T& area) : m_Area(area)
                {
                    setupTriangles();
                }

                bool isValid() const
                {
                    return m_Area.isEmpty();
                }

                bool contains() const
                {
                    return true;
                }

                bool intersects() const
                {
                    return true;
                }

                std::vector<GEOMETRY::Point<int32>> getPoints() const
                {
                    return std::vector<GEOMETRY::Point<int32>>(m_Area.pointBegin(), m_Area.pointEnd());
                }

            protected:
                T m_Area;

            private:
                std::vector<GEOMETRY::Triangle<int32>> m_Triangles;
            };
        }
    }
}
#endif
