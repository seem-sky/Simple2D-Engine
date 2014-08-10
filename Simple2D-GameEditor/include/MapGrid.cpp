#include "MapGrid.h"

using namespace MAP;
using namespace GRID;

//
//void MapGrid::_resizeGrid(const GEOMETRY::Point<uint32>3D& size)
//{
//    m_ObjectGrid.resize(boost::extents[size.x][size.y][size.z]);
//    m_GridSize = size;
//}
//
//void MapGrid::updateGridSize(GEOMETRY::Point<uint32>3D mapSize)
//{
//    _resizeGrid(GEOMETRY::Point<uint32>3D(uint32(ceil(float(mapSize.x)/GRID_SIZE)), uint32(ceil(float(mapSize.y)/GRID_SIZE)), mapSize.z));
//}
//
//bool MapGrid::_isValidGrid(const GEOMETRY::Point<uint32>3D& grid)
//{
//    if (grid.x < m_GridSize.x && grid.y < m_GridSize.y && grid.z < m_GridSize.z)
//        return true;
//    return false;
//}
//
//GEOMETRY::Point<uint32>3D MapGrid::getGridFromMapPos(const GEOMETRY::Point<uint32>3D& mapPos)
//{
//    return GEOMETRY::Point<uint32>3D(mapPos.x/GRID_SIZE, mapPos.y/GRID_SIZE, mapPos.z);
//}
//
void MapGrid::addObject(OBJECT::WorldObject* pObject)
{
    if (pObject && !m_WorldObjects.hasValidItem(pObject->getGUID()))
        m_WorldObjects.setItem(pObject->getGUID(), pObject);
}

void MapGrid::removeObject(OBJECT::WorldObject* pObject)
{
    if (pObject)
        m_WorldObjects.setItem(pObject->getGUID(), pObject);
}