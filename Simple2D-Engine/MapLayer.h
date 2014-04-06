#ifndef MAP_LAYER_H
#define MAP_LAYER_H

#include <Global.h>
#include <boost/multi_array.hpp>
#include <boost/dynamic_bitset.hpp>
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
        MapTileInfo(const MapTile& tile, UInt32Point at);

        MapTile getMapTile() const { return m_Tile; }
        UInt32Point getPosition() const { return m_Position; }

    private:
        MapTile m_Tile;
        UInt32Point m_Position;
    };

    class LayerContainer;
    class Layer
    {
        friend class LayerContainer;
    private:
        void _resize(const UInt32Point& size);
        void _clear();

    public:
        Layer(const UInt32Point& size = UInt32Point());

        inline UInt32Point getSize() const { return m_Size; }

        MapTile& getMapTile(const UInt32Point& at);
        const MapTile& getMapTile(const UInt32Point& at) const;
        MapTileInfo getMapTileInfo(const UInt32Point& at) const;

        void setMapTile(const UInt32Point& at, MapTile tile);

        inline bool isInMap(const UInt32Point& at) const { return at.x < getSize().x && at.y < getSize().y; }

    private:
        boost::multi_array<MapTile, 2> m_Layer;
        UInt32Point m_Size;
    };

    class LayerContainer
    {
    public:
        void clear();

        void resize(const UInt32Point& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize);
        inline UInt32Point getSize() const { return m_Size; }
        uint8 getLayerSize(LayerType layer) const;

        const Layer& getLayer(LayerType layer, uint8 index) const;
        Layer& getLayer(LayerType layer, uint8 index);

        MapTile& getMapTile(const UInt32Point3D& at, LayerType layer);
        const MapTile& getMapTile(const UInt32Point3D& at, LayerType layer) const;

        void setMapTile(const UInt32Point3D& at, LayerType layer, MapTile tile);

        inline bool isInMap(const UInt32Point& at) const { return at.x < getSize().x && at.y < getSize().y; }

        enum RESULT_FLAG
        {
            FLAG_NOTHING    = 0x0,
            FLAG_SAME       = 0x1,
            FLAG_OTHER      = 0x2,
            FLAG_ALL        = FLAG_SAME | FLAG_OTHER
        };
        uint32 checkAutoTiles(uint32 uiID, const UInt32Point3D& pos, UInt32PointVector& result, LayerType layer, uint32 resultFlag);

    private:
        typedef std::vector<Layer> LayerVector;
        LayerVector m_BackgroundLayer;
        LayerVector m_ForegroundLayer;

        UInt32Point m_Size;
    };
}
#endif
