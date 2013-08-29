#ifndef MAP_MGR_H
#define MAP_MGR_H

#include "MapDatabase.h"
#include "Map.h"
#include "Container.h"

namespace MAP
{
    class MapMgr : public GapsContainer<Map>
    {
    public:
        MapMgr();

        void setDatabaseMgr(DATABASE::ConstDatabaseMgrPtr pDBMgr);

        MapPtr loadMap(uint32 uiID);
        void unloadMap(MapPtr pMap);
        void unloadMap(uint32 GUID);

        void updateMaps(uint32 uiDiff);

    private:
        DATABASE::ConstDatabaseMgrPtr m_pDBMgr;
        typedef std::list<MapPtr> MapList;
        MapList m_Maps;
    };
}
#endif