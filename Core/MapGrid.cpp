//#include "MapGrid.h"
//
//using namespace MAP;
//
//void MapGrid::_resizeGrid(const Point3D<uint32> &size)
//{
//    m_ObjectGrid.resize(boost::extents[size.x][size.y][size.z]);
//    m_GridSize = size;
//}
//
//void MapGrid::updateGridSize(Point3D<uint32> mapSize)
//{
//    _resizeGrid(Point3D<uint32>(uint32(ceil(float(mapSize.x)/GRID_SIZE)), uint32(ceil(float(mapSize.y)/GRID_SIZE)), mapSize.z));
//}
//
//bool MapGrid::_isValidGrid(const Point3D<uint32> &grid)
//{
//    if (grid.x < m_GridSize.x && grid.y < m_GridSize.y && grid.z < m_GridSize.z)
//        return true;
//    return false;
//}
//
//Point3D<uint32> MapGrid::getGridFromMapPos(const Point3D<uint32> &mapPos)
//{
//    return Point3D<uint32>(mapPos.x/GRID_SIZE, mapPos.y/GRID_SIZE, mapPos.z);
//}
//
//void MapGrid::addObject(MapObjectPtr pObject)
//{
//    if (!pObject)
//        return;
//    Point3D<uint32> grid = getGridFromMapPos(pObject->m_Position);
//    if (!_isValidGrid(grid))
//        return;
//    m_ObjectGrid[grid.x][grid.y][grid.z].push_back(pObject);
//}
//
//void MapGrid::removeObject(MAP::MapObjectPtr pObject, const Point3D<uint32> &grid)
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