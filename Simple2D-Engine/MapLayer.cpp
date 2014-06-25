#include "MapLayer.h"
#include "DatabasePrototypes.h"
#include "MapException.h"

using namespace MAP;
using namespace DATABASE::PROTOTYPE::AUTO_TILE;

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

MapTileInfo Layer::getMapTile(const UInt32Point& at) const
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

void Layer::setMapTile(const MapTileInfo& tileInfo)
{
    setMapTile(tileInfo.getPosition(), tileInfo.getMapTile());
}

uint32 Layer::checkAutoTiles(uint32 uiID, const UInt32Point& pos, UInt32PointVector& result, uint32 resultFlag)
{
    uint32 uiBorderCheck = 0;
    for (uint32 i = 0; i < 8; ++i)
    {
        TILE_CHECK curTileCheck = SAME_AROUND;
        // set position check
        UInt32Point3D checkPos = pos;
        switch (i)
        {
        case 0: // top-left
            if (!checkPos.x || !checkPos.y)
                continue;
            --checkPos.x;
            --checkPos.y;
            curTileCheck = OTHER_TOP_LEFT;
            break;

        case 1: // top
            if (!checkPos.y)
                continue;
            --checkPos.y;
            curTileCheck = OTHER_TOP;
            break;

        case 2: // top-right
            if (!checkPos.y || checkPos.x+1 >= getSize().x)
                continue;
            ++checkPos.x;
            --checkPos.y;
            curTileCheck = OTHER_TOP_RIGHT;
            break;

        case 3: // left
            if (!checkPos.x)
                continue;
            --checkPos.x;
            curTileCheck = OTHER_LEFT;
            break;

        case 4: // right
            if (checkPos.x+1 >= getSize().x)
                continue;
            ++checkPos.x;
            curTileCheck = OTHER_RIGHT;
            break;

        case 5: // bottom-left
            if (!checkPos.x || checkPos.y+1 >= getSize().y)
                continue;
            --checkPos.x;
            ++checkPos.y;
            curTileCheck = OTHER_BOTTOM_LEFT;
            break;

        case 6: // bottom
            if (checkPos.y+1 >= getSize().y)
                continue;
            ++checkPos.y;
            curTileCheck = OTHER_BOTTOM;
            break;

        case 7: // bottom-right
            if (checkPos.x+1 >= getSize().x || checkPos.y+1 >= getSize().y)
                continue;
            ++checkPos.x;
            ++checkPos.y;
            curTileCheck = OTHER_BOTTOM_RIGHT;
            break;
        }
        auto mapTile = getMapTile(checkPos);
        // if bad object, continue
        if (!mapTile.getMapTile().isValid())
            continue;

        // if not same
        if (mapTile.getMapTile().m_uiAutoTileSetID != uiID)
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

MapTileInfo Layer::getBorderTileInfo(const UInt32Point& pos, BorderTile borderTile) const
{
    UInt32Point3D checkPos = pos;
    switch (borderTile)
    {
    case BorderTile::TOP_LEFT:
        --checkPos.x;
        --checkPos.y;
        break;

    case BorderTile::TOP:
        --checkPos.y;
        break;

    case BorderTile::TOP_RIGHT:
        ++checkPos.x;
        --checkPos.y;
        break;

    case BorderTile::LEFT:
        --checkPos.x;
        break;

    case BorderTile::RIGHT:
        ++checkPos.x;
        break;

    case BorderTile::BOTTOM_LEFT:
        --checkPos.x;
        ++checkPos.y;
        break;

    case BorderTile::BOTTOM:
        ++checkPos.y;
        break;

    case BorderTile::BOTTOM_RIGHT:
        ++checkPos.x;
        ++checkPos.y;
        break;
    }
    return getMapTile(checkPos);
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
    case LayerType::LAYER_BACKGROUND: return static_cast<uint8>(m_BackgroundLayer.size());
    case LayerType::LAYER_FOREGROUND: return static_cast<uint8>(m_ForegroundLayer.size());
    }
    return 0;
}
