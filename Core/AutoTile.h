#ifndef AUTO_TILE_PIXMAPS_H
#define AUTO_TILE_PIXMAPS_H

#include "QtGlobal.h"
#include "TileCache.h"

class AutoTile
{
private:
    inline void _setPixmap(DATABASE::AUTO_TILE::AUTO_TILE_INDEX index, ConstQPixmapPtr pixmap)
    {
        if (index < DATABASE::AUTO_TILE::INDEX_NONE)
            m_pPixmaps[index] = pixmap;
    }

    void _createPixmaps(ConstTileCachePtr pTileCache);
    void _clearPixmaps()
    {
        for (uint32 i = 0; i < DATABASE::AUTO_TILE::INDEX_NONE; ++i)
            m_pPixmaps[i] = QPixmapPtr();
    }

public:
    inline bool getPixmap(DATABASE::AUTO_TILE::AUTO_TILE_INDEX index, ConstQPixmapPtr &result) const
    {
        if (index >= DATABASE::AUTO_TILE::INDEX_NONE || !m_pPixmaps[index])
            return false;
        result = m_pPixmaps[index];
        return true;
    }

    inline void setAutoTilePrototype(DATABASE::ConstAutoTilePrototypePtr proto, ConstTileCachePtr pTileCache) { m_pProto = proto; _createPixmaps(pTileCache); }

private:
    ConstQPixmapPtr m_pPixmaps[DATABASE::AUTO_TILE::INDEX_NONE];
    DATABASE::ConstAutoTilePrototypePtr m_pProto;
};
typedef boost::shared_ptr<AutoTile> AutoTilePtr;
typedef boost::shared_ptr<const AutoTile> ConstAutoTilePtr;
typedef std::vector<AutoTilePtr> AutoTilePtrVector;
typedef Container<AutoTile> AutoTileContainer;

#endif