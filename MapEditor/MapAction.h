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
        TileMapAction(const Point3D<uint32> &pos, const uint32 &uiTileID, const MapPrototypePtr &map);

        void revertMapAction();

    private:
        Point3D<uint32> m_Pos;
        uint32 m_uiTileID;
    };

    class MultiTileMapAction : public MapAction
    {
    public:
        MultiTileMapAction(const uint32 &uiTileID, const MapPrototypePtr &map);

        void revertMapAction();
        void addPosition(const Point3D<uint32> &pos);

    private:
        UInt32Point3DVector m_Positions;
        uint32 m_uiTileID;
    };
}
#endif