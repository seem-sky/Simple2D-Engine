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
    inline const QPixmap* getPixmap(DATABASE::AUTO_TILE::AUTO_TILE_INDEX index) const
    {
        if (index >= DATABASE::AUTO_TILE::INDEX_NONE || !m_pPixmaps.at(index))
            return nullptr;
        return m_pPixmaps.at(index);
    }

    inline void setAutoTilePrototype(const DATABASE::AUTO_TILE::AutoTilePrototype *proto) { m_pProto = proto; _createPixmaps(); }

private:
    std::array<const QPixmap*, DATABASE::AUTO_TILE::INDEX_NONE> m_pPixmaps;
    const DATABASE::AUTO_TILE::AutoTilePrototype *m_pProto;
};

class AutoTileV2
{
private:
    // create functions
    void _createSides();
    void _createEndSides();
    void _createTs();
    void _createCurves();
    void _createInnerEdges(const DATABASE::TilePrototype *pInnerCenterTile, const DATABASE::TilePrototype *pCenterTile) const;
    void _createDiagonalInnerEdges();
    void _createMultiInnerEdges(const DATABASE::TilePrototype *pInnerCenterTile, const DATABASE::TilePrototype *pCenterTile) const;
    void _createCircle();
    void _createYs();
    void _createInnerEdgeYs();

public:
    AutoTileV2(uint32 ID, const DATABASE::DatabaseMgr &DBMgr);

    bool getPixmap(DATABASE::AUTO_TILE::AUTO_TILE_INDEX index, QPixmap &result) const;

private:
    uint32 m_AutoTileID;
    const DATABASE::DatabaseMgr &m_DBMgr;
};
#endif
