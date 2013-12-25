#ifndef MAP_LAYER_H
#define MAP_LAYER_H

#include <Global.h>
#include <boost/multi_array.hpp>
#include <boost/dynamic_bitset.hpp>

namespace DATABASE
{
    // index typedefs
    typedef uint16 TILE_INDEX;
    typedef uint8 AUTO_TILE_INDEX;
}

namespace MAP
{
    enum Layer
    {
        LAYER_BACKGROUND,
        LAYER_FOREGROUND
    };

    class MapTile
    {
    public:
        MapTile(DATABASE::TILE_INDEX uiTileID = 0, DATABASE::AUTO_TILE_INDEX uiAutoTileSetID = 0) : m_uiAutoTileSetID(uiAutoTileSetID), m_uiTileID(uiTileID) {}
        DATABASE::TILE_INDEX m_uiTileID;
        DATABASE::AUTO_TILE_INDEX m_uiAutoTileSetID;

        inline bool isAutoTile() const { return m_uiAutoTileSetID != 0; }
        inline bool isEmpty() const { return !m_uiTileID& & !m_uiAutoTileSetID; }
        inline bool isValid() const { return m_uiTileID != MATH::maximum<DATABASE::TILE_INDEX>()& &
            m_uiAutoTileSetID != MATH::maximum<DATABASE::AUTO_TILE_INDEX>(); }
    };

    typedef boost::multi_array<MapTile, 3> TileDataMultiarray3D;
    typedef std::vector<boost::dynamic_bitset<>> BitsetVector;
    class MapLayer
    {
    public:
        void clear();

        void resize(const UInt32Point& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize);
        inline UInt32Point getSize() const { return m_Size; }
        inline uint8 getLayerSize(Layer layer) const { return m_uiLayer.at(layer); };

        MapTile& getMapTile(const UInt32Point3D& at, Layer layer);
        const MapTile& getMapTile(const UInt32Point3D& at, Layer layer) const;

        void setMapTile(const UInt32Point3D& at, Layer layer, MapTile tile);

        inline bool isInMap(const UInt32Point& at) const { return at.x < getSize().x& & at.y < getSize().y; }

    private:
        TileDataMultiarray3D m_BackgroundTiles;
        TileDataMultiarray3D m_ForegroundTiles;

        UInt32Point m_Size;
        std::array<uint8, 2> m_uiLayer;
    };
}
#endif