#include "MapLayer.h"

using namespace MAP;

void MapLayer::clear()
{
    resize(UInt32Point(), 0, 0);
}

void MapLayer::resize(const UInt32Point& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize)
{
    m_Size = size;
    m_uiLayer.at(LAYER_FOREGROUND) = uiForegroundLayerSize;
    m_uiLayer.at(LAYER_BACKGROUND) = uiBackgroundLayerSize;

    m_BackgroundTiles.resize(boost::extents[m_Size.x][m_Size.y][m_uiLayer.at(LAYER_BACKGROUND)]);
    m_ForegroundTiles.resize(boost::extents[m_Size.x][m_Size.y][m_uiLayer.at(LAYER_FOREGROUND)]);
}

const MapTile& MapLayer::getMapTile(const UInt32Point3D& at, Layer layer) const
{
    if (isInMap(at) && getLayerSize(layer) > at.z)
    {
        switch (layer)
        {
        case LAYER_BACKGROUND: return m_BackgroundTiles[at.x][at.y][at.z];
        case LAYER_FOREGROUND: return m_ForegroundTiles[at.x][at.y][at.z];
        }
    }
    throw std::out_of_range(nullptr);
}

MapTile& MapLayer::getMapTile(const UInt32Point3D& at, Layer layer)
{
    return const_cast<MapTile&>(const_cast<const MapLayer&>(*this).getMapTile(at, layer));
}

void MapLayer::setMapTile(const UInt32Point3D& at, Layer layer, MapTile tile)
{
    if (isInMap(at) && getLayerSize(layer) > at.z)
    {
        switch (layer)
        {
        case LAYER_BACKGROUND:
            m_BackgroundTiles[at.x][at.y][at.z] = tile;
            break;
        case LAYER_FOREGROUND:
            m_ForegroundTiles[at.x][at.y][at.z] = tile;
            break;
        }
    }
    else
        throw std::out_of_range(nullptr);
}