#ifndef MAP_MGR_H
#define MAP_MGR_H

#include "Manager.h"
#include "MapDatabase.h"
#include "Map.h"

namespace MAP
{
    class MapMgr : public Manager<Map>
    {
    public:
        MapMgr();

        void setDatabaseMgr(DATABASE::ConstDatabaseMgrPtr pDBMgr);

        MapPtr loadMap(uint32 uiID) const;

    private:
        DATABASE::ConstDatabaseMgrPtr m_pDBMgr;
    };
}
#endif