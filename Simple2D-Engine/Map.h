#ifndef MAP_H
#define MAP_H

#include "MapLayer.h"
#include "DatabaseMgr.h"
#include "TransformationHolder.h"

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
    class Map
    {
    public:
        Map(uint32 uiID, MapLayer &layer);

        void update(uint32 uiDiff);
        void generatePassabilityMap();

        inline uint32 getWidth() const { return getSize().x; }
        inline uint32 getHeight() const { return getSize().y; }
        inline UInt32Point getSize() const { return m_Layer.getSize(); }
        inline uint32 getLayerSize(Layer layer) const { return m_Layer.getLayerSize(layer); }

        inline MapTile getMapTile(UInt32Point3D pos, Layer layer) const { return m_Layer.getMapTile(pos, layer); }

        inline Int32Point getMapPosition() const { return m_Position; }

    private:
        MapLayer m_Layer;
        std::string m_MapName;
        std::string m_ScriptName;
        uint32 m_uiID;
        Int32Point m_Position;
        TRANSFORMATION::TransformationHolder m_TransformationHolder;
    };
    typedef std::unique_ptr<Map> MapPtr;

    bool isLayerPassable(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer, UInt32Point pos, Layer layer);
    BitsetVector generatePassabilityMap(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer);
}
#endif