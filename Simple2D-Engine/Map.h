#ifndef MAP_H
#define MAP_H

#include "MapLayer.h"
#include "DatabaseMgr.h"
#include "Object.h"
#include "MapGrid.h"

//namespace DATABASE
//{
//    namespace MAP_STRUCTURE
//    {
//        class MAP_STRUCTURE::MapPrototype;
//        typedef std::shared_ptr<MapPrototype> MapPrototypePtr;
//    }
//}

namespace MAP
{
    typedef std::unordered_set<OBJECT::WorldObjectPtr> WorldObjectPtrUSet;
    class Map : public ENTITY::Object
    {
    private:
        void _setupObjects(const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects);

    public:
        Map(uint32 GUID, uint32 uiID, const std::string &name, MapLayer &layer, const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects,
            DATABASE::ConstDatabaseMgrPtr pDBMgr);

        void update(uint32 uiDiff);

        inline uint32 getWidth() const { return getSize().x; }
        inline uint32 getHeight() const { return getSize().y; }
        inline UInt32Point getSize() const { return m_Layer.getSize(); }
        inline uint32 getLayerSize(Layer layer) const { return m_Layer.getLayerSize(layer); }

        inline MapTile getMapTile(UInt32Point3D pos, Layer layer) const { return m_Layer.getMapTile(pos, layer); }

        inline const WorldObjectPtrUSet& getNewWorldObjects() const { return m_NewObjects; }

        void addWorldObject(OBJECT::WorldObjectPtr pWorldObject);

    private:
        MapLayer m_Layer;
        GRID::MapGrid m_Grid;
        DATABASE::ConstDatabaseMgrPtr m_pDBMgr;
        WorldObjectPtrUSet m_NewObjects;
    };
    typedef std::unique_ptr<Map> MapPtr;

    bool isLayerPassable(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer, UInt32Point pos, Layer layer);
    BitsetVector generatePassabilityMap(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer);
}
#endif