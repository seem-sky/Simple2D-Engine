#include "MapLayer.h"

using namespace MAP;

MapLayer::MapLayer()
{
    m_uiLayer.fill(0);
}

void MapLayer::resizeMap(UInt32Point size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize)
{
    m_BackgroundTiles.resize(boost::extents[size.x][size.y][uiBackgroundLayerSize]);
    m_ForegroundTiles.resize(boost::extents[size.x][size.y][uiForegroundLayerSize]);
}

void MapLayer::setSize(UInt32Point size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize)
{
    m_Size = size;
    m_uiLayer.at(LAYER_FOREGROUND) = uiForegroundLayerSize;
    m_uiLayer.at(LAYER_BACKGROUND) = uiBackgroundLayerSize;
}

const MapTile& MapLayer::getMapTile(UInt32Point3D at, Layer layer) const
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

MapTile& MapLayer::getMapTile(UInt32Point3D at, Layer layer)
{
    return const_cast<MapTile&>(const_cast<const MapLayer&>(*this).getMapTile(at, layer));
}