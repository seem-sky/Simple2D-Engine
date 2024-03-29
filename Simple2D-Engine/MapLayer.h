#ifndef MAP_LAYER_H
#define MAP_LAYER_H

#include "MapTile.h"
#include <geometry/Size.h>

namespace MAP
{
    class LayerContainer;
    class Layer
    {
        friend class LayerContainer;
    private:
        void _resize(const GEOMETRY::Size<uint32>& size);
        void _clear();

    public:
        Layer(const GEOMETRY::Size<uint32>& size = GEOMETRY::Size<uint32>());

        inline GEOMETRY::Size<uint32> getSize() const { return m_Size; }

        MapTileInfo getMapTile(const GEOMETRY::Point<uint32>& at) const;
        void setMapTile(const GEOMETRY::Point<uint32>& at, MapTile tile);
        void setMapTile(const MapTileInfo& tileInfo);

        inline bool isInMap(const GEOMETRY::Point<uint32>& at) const { return at.getX() < getSize().getWidth() && at.getY() < getSize().getHeight(); }

        enum RESULT_FLAG
        {
            FLAG_NOTHING    = 0x0,
            FLAG_SAME       = 0x1,
            FLAG_OTHER      = 0x2,
            FLAG_ALL        = FLAG_SAME | FLAG_OTHER
        };

        MapTileInfo getBorderTileInfo(const GEOMETRY::Point<uint32>& pos, BorderTile borderTile) const;
        uint32 checkAutoTile(const GEOMETRY::Point<uint32>& pos) const;

    private:
        std::vector<MapTileVec> m_Layer;
        GEOMETRY::Size<uint32> m_Size;
    };

    class LayerContainer
    {
    public:
        void clear();

        void resize(const GEOMETRY::Size<uint32>& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize);
        inline GEOMETRY::Size<uint32> getSize() const { return m_Size; }
        uint8 getLayerSize(LayerType layer) const;

        const Layer& getLayer(LayerType layer, uint8 index) const;
        Layer& getLayer(LayerType layer, uint8 index);

        inline bool isInMap(const GEOMETRY::Point<uint32>& at) const { return at.getX() < getSize().getWidth() && at.getY() < getSize().getHeight(); }

    private:
        typedef std::vector<Layer> LayerVector;
        LayerVector m_BackgroundLayer;
        LayerVector m_ForegroundLayer;

        GEOMETRY::Size<uint32> m_Size;
    };
}
#endif
