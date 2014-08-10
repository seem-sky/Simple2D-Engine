#ifndef MAP_TILE_H
#define MAP_TILE_H

#include "Simple2D-Global.h"
#include <math_extensions.h>
#include <vector>
#include <geometry/Point.h>

namespace MAP
{
    enum class BorderTile
    {
        TOP_LEFT,
        TOP,
        TOP_RIGHT,
        LEFT,
        RIGHT,
        BOTTOM_LEFT,
        BOTTOM,
        BOTTOM_RIGHT
    };

    class MapTile
    {
    public:
        MapTile(DATABASE::PROTOTYPE::TILE_INDEX tileID = 0-1,
            DATABASE::PROTOTYPE::AUTO_TILE_INDEX autoTileSetID = 0-1)
            : m_uiAutoTileSetID(autoTileSetID), m_uiTileID(tileID) {}
        DATABASE::PROTOTYPE::TILE_INDEX m_uiTileID = 0-1;
        DATABASE::PROTOTYPE::AUTO_TILE_INDEX m_uiAutoTileSetID = 0-1;

        inline bool isAutoTile() const { return m_uiAutoTileSetID != 0; }
        inline bool isEmpty() const { return !m_uiTileID && !m_uiAutoTileSetID; }
        inline bool isValid() const { return m_uiTileID != MATH::maximum<DATABASE::PROTOTYPE::TILE_INDEX>() &&
            m_uiAutoTileSetID != MATH::maximum<DATABASE::PROTOTYPE::AUTO_TILE_INDEX>(); }
    };
    typedef std::vector<MapTile> MapTileVec;

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
        MapTileInfo(const MapTile& tile = MapTile(), GEOMETRY::Point<uint32> at = GEOMETRY::Point<uint32>()) : m_Tile(tile), m_Position(at) {}

        inline const MapTile& getMapTile() const { return m_Tile; }
        inline MapTile& getMapTile() { return m_Tile; }
        inline GEOMETRY::Point<uint32> getPosition() const { return m_Position; }

        inline bool isValid() const { return m_Tile.isValid(); }
        inline bool isAutoTile() const { return m_Tile.isAutoTile(); }

    private:
        MapTile m_Tile;
        GEOMETRY::Point<uint32> m_Position;
    };
    typedef std::vector<MapTileInfo> MapTileInfoVec;

    static DATABASE::PROTOTYPE::AUTO_TILE::TILE_CHECK borderTileToTileCheck(BorderTile borderTile)
    {
        uint32 check = 1;
        check <<= static_cast<uint32>(borderTile);
        return static_cast<DATABASE::PROTOTYPE::AUTO_TILE::TILE_CHECK>(check);
    }
}
#endif
