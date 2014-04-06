#include "MapLayer.h"
#include "DatabasePrototypes.h"
#include "MapException.h"

using namespace MAP;

/*#####
# MapTileInfo
#####*/
MapTileInfo::MapTileInfo(const MapTile& tile, UInt32Point at) : m_Tile(tile), m_Position(at)
{
}

/*#####
# Layer
#####*/
Layer::Layer(const UInt32Point& size)
{
    _resize(size);
}

void Layer::_resize(const UInt32Point& size)
{
    m_Size = size;
    m_Layer.resize(boost::extents[m_Size.x][m_Size.y]);
}

void Layer::_clear()
{
    _resize(UInt32Point());
}

const MapTile& Layer::getMapTile(const UInt32Point& at) const
{
    if (isInMap(at))
        return m_Layer[at.x][at.y];
    throw EXCEPTION::TileRangeException(nullptr);
}

MapTile& Layer::getMapTile(const UInt32Point& at)
{
    return const_cast<MapTile&>(const_cast<const Layer&>(*this).getMapTile(at));
}

MapTileInfo Layer::getMapTileInfo(const UInt32Point& at) const
{
    if (isInMap(at))
        return MapTileInfo(m_Layer[at.x][at.y], at);
    throw EXCEPTION::TileRangeException(nullptr);
}

void Layer::setMapTile(const UInt32Point& at, MapTile tile)
{
    if (isInMap(at))
        m_Layer[at.x][at.y] = tile;
    else
        throw EXCEPTION::TileRangeException(nullptr);
}

/*#####
# LayerContainer
#####*/
void LayerContainer::clear()
{
    for (auto& layer : m_BackgroundLayer)
        layer._clear();
    for (auto& layer : m_ForegroundLayer)
        layer._clear();
}

void LayerContainer::resize(const UInt32Point& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize)
{
    m_Size = size;
    for (auto& layer : m_BackgroundLayer)
        layer._resize(size);
    for (auto& layer : m_ForegroundLayer)
        layer._resize(size);
    m_BackgroundLayer.resize(uiBackgroundLayerSize, Layer(size));
    m_ForegroundLayer.resize(uiForegroundLayerSize, Layer(size));
}

const Layer& LayerContainer::getLayer(LayerType layer, uint8 index) const
{
    if (getLayerSize(layer) > index)
    {
        switch (layer)
        {
        case LayerType::LAYER_BACKGROUND: return m_BackgroundLayer.at(index);
        case LayerType::LAYER_FOREGROUND: return m_ForegroundLayer.at(index);
        }
    }
    throw std::out_of_range(nullptr);
}

Layer& LayerContainer::getLayer(LayerType layer, uint8 index)
{
    return const_cast<Layer&>(const_cast<const LayerContainer&>(*this).getLayer(layer, index));
}

uint8 LayerContainer::getLayerSize(LayerType layer) const
{
    switch (layer)
    {
    case MAP::LayerType::LAYER_BACKGROUND: return static_cast<uint8>(m_BackgroundLayer.size());
    case MAP::LayerType::LAYER_FOREGROUND: return static_cast<uint8>(m_ForegroundLayer.size());
    }
    return 0;
}

const MapTile& LayerContainer::getMapTile(const UInt32Point3D& at, LayerType layer) const
{
    return getLayer(layer, at.z).getMapTile(at);
}

MapTile& LayerContainer::getMapTile(const UInt32Point3D& at, LayerType layer)
{
    return getLayer(layer, at.z).getMapTile(at);
}

void LayerContainer::setMapTile(const UInt32Point3D& at, LayerType layer, MapTile tile)
{
    getLayer(layer, at.z).setMapTile(at, tile);
}

uint32 LayerContainer::checkAutoTiles(uint32 uiID, const UInt32Point3D& pos, UInt32PointVector& result, LayerType layer, uint32 resultFlag)
{
    uint32 uiBorderCheck = 0;
    MapTile centerTile = getMapTile(pos, layer);
    for (uint32 i = 0; i < 8; ++i)
    {
        DATABASE::AUTO_TILE::TILE_CHECK curTileCheck = DATABASE::AUTO_TILE::SAME_AROUND;
        // set position check
        UInt32Point3D checkPos = pos;
        switch (i)
        {
        case 0: // top-left
            if (!checkPos.x || !checkPos.y)
                continue;
            --checkPos.x;
            --checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP_LEFT;
            break;

        case 1: // top
            if (!checkPos.y)
                continue;
            --checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP;
            break;

        case 2: // top-right
            if (!checkPos.y || checkPos.x+1 >= getSize().x)
                continue;
            ++checkPos.x;
            --checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP_RIGHT;
            break;

        case 3: // left
            if (!checkPos.x)
                continue;
            --checkPos.x;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_LEFT;
            break;

        case 4: // right
            if (checkPos.x+1 >= getSize().x)
                continue;
            ++checkPos.x;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_RIGHT;
            break;

        case 5: // bottom-left
            if (!checkPos.x || checkPos.y+1 >= getSize().y)
                continue;
            --checkPos.x;
            ++checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM_LEFT;
            break;

        case 6: // bottom
            if (checkPos.y+1 >= getSize().y)
                continue;
            ++checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM;
            break;

        case 7: // bottom-right
            if (checkPos.x+1 >= getSize().x || checkPos.y+1 >= getSize().y)
                continue;
            ++checkPos.x;
            ++checkPos.y;
            curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM_RIGHT;
            break;
        }
        MapTile mapTile = getMapTile(checkPos, layer);
        // if bad object, continue
        if (!mapTile.isValid())
            continue;

        // if not same
        if (mapTile.m_uiAutoTileSetID != uiID)
        {
            uiBorderCheck += curTileCheck;
            if (resultFlag & FLAG_OTHER)
            {
                result.push_back(checkPos);
            }
        }
        // if same
        else if (resultFlag & FLAG_SAME)
            result.push_back(checkPos);
    }
    return uiBorderCheck;
}
