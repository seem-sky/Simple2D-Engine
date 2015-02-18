#ifndef MAP_SCRIPT_AREA_AREA_BASE_H
#define MAP_SCRIPT_AREA_AREA_BASE_H

#include "AreaInterface.h"
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

                GEOMETRY::Point<int32> getPoint(uint32 i) const
                {
                    return m_Area.getPoint(i);
                }

                void setPoint(uint32 i, const GEOMETRY::Point<int32>& p)
                {
                    m_Area.setPoint(i, p);
                }

                uint32 pointCount() const
                {
                    return m_Area.pointCount();
                }

                virtual Data getData() const = 0;

            protected:
                T m_Area;

            private:
                std::vector<GEOMETRY::Triangle<int32>> m_Triangles;
            };
        }
    }
}
#endif
