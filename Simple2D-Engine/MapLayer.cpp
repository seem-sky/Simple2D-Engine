#include "MapLayer.h"
#include "database/Prototype/Derived.h"
#include "MapException.h"

using namespace MAP;
using namespace database::prototype;

/*#####
# Layer
#####*/
Layer::Layer(const GEOMETRY::Size<uint32>& size)
{
    _resize(size);
}

void Layer::_resize(const GEOMETRY::Size<uint32>& size)
{
    m_Size = size;
    m_Layer.resize(size.getWidth(), MapTileVec(size.getHeight(), MapTile(0, 0)));
}

void Layer::_clear()
{
    _resize(GEOMETRY::Size<uint32>());
}

MapTileInfo Layer::getMapTile(const GEOMETRY::Point<uint32>& at) const
{
    try
    {
        return MapTileInfo(m_Layer.at(at.getX()).at(at.getY()), at);
    }
    catch (std::out_of_range&)
    {
        throw EXCEPTION::TileOutOfRangeException(nullptr);
    }
}

void Layer::setMapTile(const GEOMETRY::Point<uint32>& at, MapTile tile)
{
    try
    {
        m_Layer.at(at.getX()).at(at.getY()) = tile;
    }
    catch (std::out_of_range&)
    {
        throw EXCEPTION::TileOutOfRangeException(nullptr);
    }
}

void Layer::setMapTile(const MapTileInfo& tileInfo)
{
    setMapTile(tileInfo.getPosition(), tileInfo.getMapTile());
}

MapTileInfo Layer::getBorderTileInfo(const GEOMETRY::Point<uint32>& pos, BorderTile borderTile) const
{
    GEOMETRY::Point<uint32> checkPos = pos;
    switch (borderTile)
    {
    case BorderTile::TOP_LEFT:
        --checkPos.getX();
        --checkPos.getY();
        break;

    case BorderTile::TOP:
        --checkPos.getY();
        break;

    case BorderTile::TOP_RIGHT:
        ++checkPos.getX();
        --checkPos.getY();
        break;

    case BorderTile::LEFT:
        --checkPos.getX();
        break;

    case BorderTile::RIGHT:
        ++checkPos.getX();
        break;

    case BorderTile::BOTTOM_LEFT:
        --checkPos.getX();
        ++checkPos.getY();
        break;

    case BorderTile::BOTTOM:
        ++checkPos.getY();
        break;

    case BorderTile::BOTTOM_RIGHT:
        ++checkPos.getX();
        ++checkPos.getY();
        break;
    }
    return getMapTile(checkPos);
}

uint32 Layer::checkAutoTile(const GEOMETRY::Point<uint32>& pos) const
{
    auto info = getMapTile(pos);
    if (!info.isAutoTile())
        throw EXCEPTION::IncorrectTileException("Is no auto tile.");

    uint32 check = 0;
    for (uint32 i = 0; i < 8; ++i)
    {
        auto borderTile = static_cast<BorderTile>(i);
        try
        {
            auto mapTileInfo = getBorderTileInfo(info.getPosition(), borderTile);
            if (!mapTileInfo.isValid() || info.getMapTile().m_uiAutoTileSetID == mapTileInfo.getMapTile().m_uiAutoTileSetID)
                continue;

            check |= borderTileToTileCheck(borderTile);
        }
        catch (const EXCEPTION::TileOutOfRangeException&) {}
    }
    return check;
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

void LayerContainer::resize(const GEOMETRY::Size<uint32>& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize)
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
    throw EXCEPTION::LayerOutOfRangeException(nullptr);
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
