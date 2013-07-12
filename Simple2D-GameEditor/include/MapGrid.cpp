#include "MapGrid.h"

using namespace MAP;
using namespace GRID;

//
//void MapGrid::_resizeGrid(const UInt32Point3D &size)
//{
//    m_ObjectGrid.resize(boost::extents[size.x][size.y][size.z]);
//    m_GridSize = size;
//}
//
//void MapGrid::updateGridSize(UInt32Point3D mapSize)
//{
//    _resizeGrid(UInt32Point3D(uint32(ceil(float(mapSize.x)/GRID_SIZE)), uint32(ceil(float(mapSize.y)/GRID_SIZE)), mapSize.z));
//}
//
//bool MapGrid::_isValidGrid(const UInt32Point3D &grid)
//{
//    if (grid.x < m_GridSize.x && grid.y < m_GridSize.y && grid.z < m_GridSize.z)
//        return true;
//    return false;
//}
//
//UInt32Point3D MapGrid::getGridFromMapPos(const UInt32Point3D &mapPos)
//{
//    return UInt32Point3D(mapPos.x/GRID_SIZE, mapPos.y/GRID_SIZE, mapPos.z);
//}
//
void MapGrid::addObject(OBJECT::WorldObjectPtr pObject)
{
    if (pObject && !m_WorldObjects.getItem(pObject->getGUID(), OBJECT::WorldObjectPtr()))
        m_WorldObjects.setItem(pObject->getGUID(), pObject);
}

void MapGrid::removeObject(OBJECT::WorldObjectPtr pObject)
{
    if (pObject)
        m_WorldObjects.setItem(pObject->getGUID(), pObject, false);
}