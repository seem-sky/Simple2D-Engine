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

    void _createPixmaps(const TileCache &tileCache);
    void _clearPixmaps()
    {
        for (auto &pPixmap : m_pPixmaps)
            pPixmap = nullptr;
    }

public:
    inline bool getPixmap(DATABASE::AUTO_TILE::AUTO_TILE_INDEX index, const QPixmap *result) const
    {
        // ToDo: Return pointer.
        if (index >= DATABASE::AUTO_TILE::INDEX_NONE || !m_pPixmaps.at(index))
            return nullptr;
        result = m_pPixmaps.at(index);
        return true;
    }

    inline void setAutoTilePrototype(const DATABASE::AUTO_TILE::AutoTilePrototype *proto, const TileCache &tileCache) { m_pProto = proto; _createPixmaps(tileCache); }

private:
    std::array<const QPixmap*, DATABASE::AUTO_TILE::INDEX_NONE> m_pPixmaps;
    const DATABASE::AUTO_TILE::AutoTilePrototype *m_pProto;
};
#endif
