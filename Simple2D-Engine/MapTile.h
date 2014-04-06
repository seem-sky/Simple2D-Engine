#ifndef MAP_TILE_H
#define MAP_TILE_H

#include "DatabasePrototypes.h"

namespace MAP
{
    class MapTile
    {
    public:
        MapTile(DATABASE::TILE_INDEX uiTileID = 0, DATABASE::AUTO_TILE_INDEX uiAutoTileSetID = 0) : m_uiAutoTileSetID(uiAutoTileSetID), m_uiTileID(uiTileID) {}
        DATABASE::TILE_INDEX m_uiTileID;
        DATABASE::AUTO_TILE_INDEX m_uiAutoTileSetID;

        inline bool isAutoTile() const { return m_uiAutoTileSetID != 0; }
        inline bool isEmpty() const { return !m_uiTileID && !m_uiAutoTileSetID; }
        inline bool isValid() const { return m_uiTileID != MATH::maximum<DATABASE::TILE_INDEX>() &&
            m_uiAutoTileSetID != MATH::maximum<DATABASE::AUTO_TILE_INDEX>(); }
    };
    static bool operator==(const MapTile& lhs, const MapTile& rhs)
    {
        if (lhs.isAutoTile() && rhs.isAutoTile())
            return lhs.m_uiAutoTileSetID == rhs.m_uiAutoTileSetID;
        else if (!lhs.isAutoTile() && !rhs.isAutoTile())
            return lhs.m_uiTileID == rhs.m_uiTileID;
        return false;
    }
    static bool operator!=(const MapTile& lhs, const MapTile& rhs) { return !(lhs == rhs); }

    class MapTileInfo
    {
    public:
        MapTileInfo(const MapTile& tile, UInt32Point at) : m_Tile(tile), m_Position(at) {}

        inline const MapTile& getMapTile() const { return m_Tile; }
        inline MapTile& getMapTile() { return m_Tile; }
        inline UInt32Point getPosition() const { return m_Position; }

    private:
        MapTile m_Tile;
        UInt32Point m_Position;
    };
}
#endif
