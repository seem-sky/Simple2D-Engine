#ifndef CACHE_MANAGER_H
#define CACHE_MANAGER_H

#include "Tiles.h"
#include "AutoTiles.h"

namespace CACHE
{
    class Manager
    {
    public:
        Manager(const database::Manager& DBMgr);

        void clear();

        Tiles& getTileCache();
        AutoTiles& getAutoTileCache();

    private:
        Tiles m_TileCache;
        AutoTiles m_AutoTileCache;
    };
}
#endif
