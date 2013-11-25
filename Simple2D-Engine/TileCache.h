#ifndef TILE_CACHE_H
#define TILE_CACHE_H

#include "QtGlobal.h"
#include "DatabaseMgr.h"

class TileCache : public Container<QPixmap>
{
private:
    QPixmap* _createPixmap(uint32 uiID);

public:
    TileCache(const DATABASE::DatabaseMgr &DBMgr);

    const QPixmap* getItem(uint32 uiID) const;

    inline const DATABASE::DatabaseMgr& getDBMgr() const { return m_DBMgr; }

private:
    const DATABASE::DatabaseMgr &m_DBMgr;
};
#endif