#ifndef MAP_LAYER_H
#define MAP_LAYER_H

#include "MapTile.h"

namespace MAP
{
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

        MapTileInfo getMapTile(const UInt32Point& at) const;
        void setMapTile(const UInt32Point& at, MapTile tile);
        void setMapTile(const MapTileInfo& tileInfo);

        inline bool isInMap(const UInt32Point& at) const { return at.getX() < getSize().getX() && at.getY() < getSize().getY(); }

        enum RESULT_FLAG
        {
            FLAG_NOTHING    = 0x0,
            FLAG_SAME       = 0x1,
            FLAG_OTHER      = 0x2,
            FLAG_ALL        = FLAG_SAME | FLAG_OTHER
        };
        uint32 checkAutoTiles(uint32 uiID, const UInt32Point& pos, UInt32PointVec& result, uint32 resultFlag);

        MapTileInfo getBorderTileInfo(const UInt32Point& pos, BorderTile borderTile) const;
        uint32 checkAutoTile(const UInt32Point& pos) const;

    private:
        std::vector<MapTileVec> m_Layer;
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

        inline bool isInMap(const UInt32Point& at) const { return at.getX() < getSize().getX() && at.getY() < getSize().getY(); }

    private:
        typedef std::vector<Layer> LayerVector;
        LayerVector m_BackgroundLayer;
        LayerVector m_ForegroundLayer;

        UInt32Point m_Size;
    };
}
#endif
