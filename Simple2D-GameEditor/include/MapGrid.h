#ifndef MAP_GRID_H
#define MAP_GRID_H

#include "Global.h"
#include "DynamicObject.h"
#include <boost/multi_array.hpp>
#include <unordered_set>

const uint32 GRID_SIZE = 10; // in tiles

namespace MAP
{
    typedef std::unordered_set<OBJECT::WorldObjectPtr> WorldObjectPtrUSet;
    typedef SpaceContainer<OBJECT::WorldObject> WorldObjectContainer;
    namespace GRID
    {
        class MapGrid
        {
        private:
            //void _resizeGrid(const UInt32Point3D &size);

            //bool _isValidGrid(const UInt32Point3D &grid);

        public:
            //static UInt32Point3D getGridFromMapPos(const UInt32Point3D &mapPos);

            //void updateGridSize(UInt32Point3D mapSize);
            //inline void clearGrid() { _resizeGrid(UInt32Point3D()); }

            void addObject(OBJECT::WorldObjectPtr pObject);
            void removeObject(OBJECT::WorldObjectPtr pObject);

            inline WorldObjectContainer& getObjects() { return m_WorldObjects; }

        private:
            WorldObjectContainer m_WorldObjects;
            UInt32Point3D m_GridSize;
        };
    }
}
#endif