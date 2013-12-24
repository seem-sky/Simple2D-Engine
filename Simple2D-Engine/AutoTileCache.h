#ifndef AUTO_TILE_CACHE_H
#define AUTO_TILE_CACHE_H

#include "AutoTile.h"
#include <Singleton.h>

class AutoTileCache : public Container<AutoTile>
{
private:
    AutoTile* _createAutoTile(uint32 uiID);

public:
    AutoTileCache();

    const AutoTile* getItem(uint32 uiID) const;

    inline void setDBMgr(const DATABASE::DatabaseMgr* pDBMgr) { m_pDBMgr = pDBMgr; }

private:
    const DATABASE::DatabaseMgr *m_pDBMgr;
};
typedef TSingleton<AutoTileCache> GAutoTileCache;

#endif
