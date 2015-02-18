#ifndef CACHE_TILE_BASE_H
#define CACHE_TILE_BASE_H

#include "TextureAtlas.h"
#include "TileCacheInfo.h"
#include "Simple2D-Global.h"
#include "DatabaseMgr.h"

namespace CACHE
{
    template <uint16 tileWidth, uint16 tileHeight, uint16 atlasWidth, uint16 atlasHeight>
    class TileBase
    {
    protected:
        TileCacheInfo add(uint32 ID, const QPixmap& pixmap)
        {
            // resize if needed
            if (ID > m_Positions.size())
                m_Positions.resize(ID);

            auto info = getEmptyPosition();
            info.first->add(info.second, pixmap);
            m_Positions.at(ID - 1) = TileCacheInfo(info.first->getTexture(), info.second);
            return m_Positions.at(ID - 1);
        }

        std::pair<CACHE::TextureAtlas<tileWidth, tileHeight>*, GEOMETRY::Point<uint32>> getEmptyPosition()
        {
            for (auto& atlas : m_Atlas)
            {
                try
                {
                    return std::make_pair(&atlas, atlas.getEmptyPosition());
                }
                catch (const std::runtime_error&) {}
            }

            // create new atlas
            m_Atlas.push_back(CACHE::TextureAtlas<tileWidth, tileHeight>(atlasWidth, atlasHeight));
            return std::make_pair(&m_Atlas.back(), m_Atlas.back().getEmptyPosition());
        }

    public:
        TileBase(const DATABASE::DatabaseMgr& DBMgr)
            : m_DBMgr(DBMgr)
        {}

        bool isEmpty(uint32 ID) const
        {
            return !ID || m_Positions.size() < ID || !m_Positions.at(ID - 1).isValid();
        }

        virtual void clear()
        {
            m_Atlas.clear();
            m_Positions.clear();
        }

    protected:
        const DATABASE::DatabaseMgr& m_DBMgr;
        std::vector<TextureAtlas<tileWidth, tileHeight>> m_Atlas;
        std::vector<TileCacheInfo> m_Positions;
    };
}
#endif
