#ifndef AUTO_TILE_PIXMAPS_H
#define AUTO_TILE_PIXMAPS_H

#include "TileCache.h"
#include <array>

class AutoTile
{
private:
    inline void _setPixmap(DATABASE::AUTO_TILE::AUTO_TILE_INDEX index, const QPixmap *pPixmap)
    {
        if (index < DATABASE::AUTO_TILE::INDEX_NONE)
            m_pPixmaps.at(index) = pPixmap;
    }

    void _createPixmaps();
    void _clearPixmaps()
    {
        m_pPixmaps.fill(nullptr);
    }

public:
    AutoTile(const DATABASE::DatabaseMgr &DBMgr, uint32 ID);

    inline const QPixmap* getPixmap(DATABASE::AUTO_TILE::AUTO_TILE_INDEX index) const
    {
        if (index >= DATABASE::AUTO_TILE::INDEX_NONE || !m_pPixmaps.at(index))
            return nullptr;
        return m_pPixmaps.at(index);
    }

private:
    std::array<const QPixmap*, DATABASE::AUTO_TILE::INDEX_NONE> m_pPixmaps;
    const uint32 m_ID;
    const DATABASE::DatabaseMgr &m_DBMgr;
};

#endif
