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
        MapAction(const MapPrototypePtr &map);

        virtual void revertMapAction() = 0;

        inline MapPrototypePtr getMap() const {return m_pMap; } 

    private:
        MapPrototypePtr m_pMap;
    };
    typedef boost::shared_ptr<MapAction> MapActionPtr;
    typedef std::vector<MapActionPtr> MapActionPtrVector;

    /*#####
    # TileMapAction
    #####*/
    class TileMapAction : public MapAction
    {
    public:
        TileMapAction(const Point3D<uint32> &pos, const MapTile &uiTileID, const MapPrototypePtr &map);

        void revertMapAction();

    private:
        Point3D<uint32> m_Pos;
        MapTile m_MapTile;
    };

    class MultiTileMapAction : public MapAction
    {
    public:
        MultiTileMapAction(const MapTile &mapTile, const MapPrototypePtr &map, const uint32 &uiLayer);

        void revertMapAction();
        void addPosition(const Point<uint32> &pos);

    private:
        UInt32PointVector m_Positions;
        uint32 m_uiLayer;
        MapTile m_MapTile;
    };
    typedef boost::shared_ptr<MultiTileMapAction> MultiTileMapActionPtr;
}
#endif