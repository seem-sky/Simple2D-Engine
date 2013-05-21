#ifndef MAP_GRID_H
#define MAP_GRID_H

#include "Global.h"
#include "DatabasePrototypes.h"
#include <boost/multi_array.hpp>

const uint32 GRID_SIZE = 10;

namespace MAP
{
    class MapGrid
    {
    //private:
    //    void _resizeGrid(const Point3D<uint32> &size);

    //    bool _isValidGrid(const Point3D<uint32> &grid);

    //public:
    //    static Point3D<uint32> getGridFromMapPos(const Point3D<uint32> &mapPos);

    //    void updateGridSize(Point3D<uint32> mapSize);
    //    inline void clearGrid() { _resizeGrid(Point3D<uint32>()); }

    //    void addObject(MapObjectPtr pObject);
    //    void removeObject(MapObjectPtr pObject, const Point3D<uint32> &grid);

    //private:
    //    MapObjectPtrVectorMultiarray3D m_ObjectGrid;
    //    Point3D<uint32> m_GridSize;
    };
}
#endif