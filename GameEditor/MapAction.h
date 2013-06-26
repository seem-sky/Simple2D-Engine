#ifndef MAP_ACTION_H
#define MAP_ACTION_H

#include "MapDatabase.h"

namespace MAP
{
    /*#####
    # MapAction
    #####*/
    class MapAction
    {
    public:
        MapAction(MapPrototypePtr map);

        virtual void revertMapAction() = 0;

        inline MapPrototypePtr getMap() const {return m_pMap; } 

    private:
        MapPrototypePtr m_pMap;
    };
    typedef std::shared_ptr<MapAction> MapActionPtr;
    typedef std::vector<MapActionPtr> MapActionPtrVector;

    /*#####
    # TileMapAction
    #####*/
    class TileMapAction : public MapAction
    {
    public:
        TileMapAction(Point3D<uint32> pos, MapTile uiTileID, Layer layer, MapPrototypePtr map);

        void revertMapAction();

    private:
        Point3D<uint32> m_Pos;
        MapTile m_MapTile;
        Layer m_Layer;
    };

    class MultiTileMapAction : public MapAction
    {
    public:
        MultiTileMapAction(MapTile mapTile, MapPrototypePtr map, uint32 uiLayer, Layer layer);

        void revertMapAction();
        void addPosition(const Point<uint32> &pos);

    private:
        UInt32PointVector m_Positions;
        uint32 m_uiLayer;
        MapTile m_MapTile;
        Layer m_Layer;
    };
    typedef std::shared_ptr<MultiTileMapAction> MultiTileMapActionPtr;
}
#endif