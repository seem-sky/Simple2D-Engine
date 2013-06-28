//#include "MapGrid.h"
//
//using namespace MAP;
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
//void MapGrid::addObject(MapObjectPtr pObject)
//{
//    if (!pObject)
//        return;
//    UInt32Point3D grid = getGridFromMapPos(pObject->m_Position);
//    if (!_isValidGrid(grid))
//        return;
//    m_ObjectGrid[grid.x][grid.y][grid.z].push_back(pObject);
//}
//
//void MapGrid::removeObject(MAP::MapObjectPtr pObject, const UInt32Point3D &grid)
//{
//    if (!pObject || !_isValidGrid(grid))
//        return;
//    for (MapObjectPtrVector::const_iterator itr = m_ObjectGrid[grid.x][grid.y][grid.z].begin(); itr != m_ObjectGrid[grid.x][grid.y][grid.z].end(); ++itr)
//    {
//        if (*itr == pObject)
//        {
//            m_ObjectGrid[grid.x][grid.y][grid.z].erase(itr);
//            return;
//        }
//    }
//}