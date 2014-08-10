#ifndef MAP_GRID_H
#define MAP_GRID_H

#include <Global.h>
#include "WorldObject.h"
#include <boost/multi_array.hpp>
#include <unordered_set>
#include <geometry/Point3D.h>

const uint32 GRID_SIZE = 10; // in tiles

namespace MAP
{
    typedef std::unordered_set<OBJECT::WorldObject*> WorldObjectPtrUSet;
    typedef Container<OBJECT::WorldObject> WorldObjectContainer;
    namespace GRID
    {
        class MapGrid
        {
        private:
            //void _resizeGrid(const GEOMETRY::Point<uint32>3D& size);

            //bool _isValidGrid(const GEOMETRY::Point<uint32>3D& grid);

        public:
            //static GEOMETRY::Point<uint32>3D getGridFromMapPos(const GEOMETRY::Point<uint32>3D& mapPos);

            //void updateGridSize(GEOMETRY::Point<uint32>3D mapSize);
            //inline void clearGrid() { _resizeGrid(GEOMETRY::Point<uint32>3D()); }

            void addObject(OBJECT::WorldObject* pObject);
            void removeObject(OBJECT::WorldObject* pObject);

            inline WorldObjectContainer& getObjects() { return m_WorldObjects; }

        private:
            WorldObjectContainer m_WorldObjects;
            GEOMETRY::Point3D<uint32> m_GridSize;
        };
    }
}
#endif