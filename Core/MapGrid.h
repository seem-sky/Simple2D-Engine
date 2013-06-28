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
    //    void _resizeGrid(const UInt32Point3D &size);

    //    bool _isValidGrid(const UInt32Point3D &grid);

    //public:
    //    static UInt32Point3D getGridFromMapPos(const UInt32Point3D &mapPos);

    //    void updateGridSize(UInt32Point3D mapSize);
    //    inline void clearGrid() { _resizeGrid(UInt32Point3D()); }

    //    void addObject(MapObjectPtr pObject);
    //    void removeObject(MapObjectPtr pObject, const UInt32Point3D &grid);

    //private:
    //    MapObjectPtrVectorMultiarray3D m_ObjectGrid;
    //    UInt32Point3D m_GridSize;
    };
}
#endif