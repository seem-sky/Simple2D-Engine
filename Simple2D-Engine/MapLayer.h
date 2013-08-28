#ifndef MAP_LAYER_H
#define MAP_LAYER_H

#include <Global.h>
#include <boost/multi_array.hpp>
#include <boost/dynamic_bitset.hpp>

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
        MapTile(uint32 uiTileID = 0, uint32 uiAutoTileSetID = 0) : m_uiAutoTileSetID(uiAutoTileSetID), m_uiTileID(uiTileID) {}
        uint32 m_uiTileID;
        uint32 m_uiAutoTileSetID;

        inline bool isAutoTile() const { return m_uiAutoTileSetID != 0; }
        inline bool isEmpty() const { return !m_uiTileID && !m_uiAutoTileSetID; }
        inline bool isValid() const { return m_uiTileID != MAX_UINT32 && m_uiAutoTileSetID != MAX_UINT32; }
    };

    typedef boost::multi_array<MapTile, 3> TileDataMultiarray3D;
    typedef std::vector<boost::dynamic_bitset<>> BitsetVector;
    class MapLayer
    {
    public:
        MapLayer();

        void resizeMap(UInt32Point size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize);
        void setSize(UInt32Point size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize);
        inline UInt32Point getSize() const { return m_Size; }
        inline uint32 getLayerSize(Layer layer) const { return m_uiLayer.at(layer); };

        MapTile& getMapTile(UInt32Point3D at, Layer layer);
        const MapTile& getMapTile(UInt32Point3D at, Layer layer) const;

        inline bool isInMap(UInt32Point at) const { return at.x < getSize().x && at.y < getSize().y; }

    private:
        TileDataMultiarray3D m_BackgroundTiles;
        TileDataMultiarray3D m_ForegroundTiles;

        UInt32Point m_Size;
        std::array<uint32, 2> m_uiLayer;
    };
}
#endif