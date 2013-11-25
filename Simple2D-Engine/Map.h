#ifndef MAP_H
#define MAP_H

#include "MapLayer.h"
#include "DatabaseMgr.h"
#include "MapGrid.h"
#include "Entity.h"

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
    typedef std::unordered_set<OBJECT::WorldObject*> WorldObjectPtrUSet;
    class Map : public ENTITY::Entity
    {
    private:
        void _setupObjects(const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects);

    public:
        Map(uint32 GUID, uint32 ID, MapLayer &layer, const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects, const DATABASE::DatabaseMgr &DBMgr,
            const std::string &name = "", const std::string &script = "");

        void update(uint32 uiDiff);

        inline uint32 getWidth() const { return getSize().x; }
        inline uint32 getHeight() const { return getSize().y; }
        inline UInt32Point getSize() const { return m_Layer.getSize(); }
        inline uint8 getLayerSize(Layer layer) const { return m_Layer.getLayerSize(layer); }

        inline MapTile getMapTile(UInt32Point3D pos, Layer layer) const { return m_Layer.getMapTile(pos, layer); }

        inline const WorldObjectPtrUSet& getNewWorldObjects() const { return m_NewObjects; }

        void addWorldObject(OBJECT::WorldObject *pWorldObject);
        inline OBJECT::WorldObject* getWorldObject(uint32 GUID) { return m_Grid.getObjects().getItem(GUID); }

    private:
        MapLayer m_Layer;
        GRID::MapGrid m_Grid;
        const DATABASE::DatabaseMgr &m_DBMgr;
        WorldObjectPtrUSet m_NewObjects;
    };
    typedef std::shared_ptr<Map> MapPtr;
    typedef std::shared_ptr<const Map> ConstMapPtr;

    //bool isLayerPassable(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer, UInt32Point pos, Layer layer);
    //BitsetVector generatePassabilityMap(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer);
}
#endif