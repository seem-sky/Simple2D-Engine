#ifndef TILE_CACHE_H
#define TILE_CACHE_H

#include "QtGlobal.h"
#include "DatabaseMgr.h"
#include <Singleton.h>

class TileCache : public Container<QPixmap>
{
private:
    QPixmap* _createPixmap(uint32 uiID);

public:
    TileCache();

    const QPixmap* getItem(uint32 uiID) const;

    inline void setDBMgr(const DATABASE::DatabaseMgr* pDBMgr) { m_pDBMgr = pDBMgr; }

private:
    const DATABASE::DatabaseMgr* m_pDBMgr;
};
typedef TSingleton<TileCache> GTileCache;

#endif
