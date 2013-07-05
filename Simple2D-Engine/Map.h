#ifndef MAP_H
#define MAP_H

#include "MapLayer.h"
#include "DatabaseMgr.h"
#include "Object.h"

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
    class Map : public ENTITY::Object
    {
    public:
        Map(uint32 uiID, const std::string &name, MapLayer &layer);

        virtual void update(uint32 uiDiff);

        inline uint32 getWidth() const { return getSize().x; }
        inline uint32 getHeight() const { return getSize().y; }
        inline UInt32Point getSize() const { return m_Layer.getSize(); }
        inline uint32 getLayerSize(Layer layer) const { return m_Layer.getLayerSize(layer); }

        inline MapTile getMapTile(UInt32Point3D pos, Layer layer) const { return m_Layer.getMapTile(pos, layer); }

    private:
        MapLayer m_Layer;
        std::string m_ScriptName;
    };
    typedef std::unique_ptr<Map> MapPtr;

    bool isLayerPassable(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer, UInt32Point pos, Layer layer);
    BitsetVector generatePassabilityMap(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer);
}
#endif