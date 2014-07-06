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
    m_Layer.resize(size.getX(), MapTileVec(size.getY(), MapTile(0, 0)));
}

void Layer::_clear()
{
    _resize(UInt32Point());
}

MapTileInfo Layer::getMapTile(const UInt32Point& at) const
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

void Layer::setMapTile(const UInt32Point& at, MapTile tile)
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

uint32 Layer::checkAutoTiles(uint32 uiID, const UInt32Point& pos, UInt32PointVec& result, uint32 resultFlag)
{
    uint32 uiBorderCheck = 0;
    //for (uint32 i = 0; i < 8; ++i)
    //{
    //    TILE_CHECK curTileCheck = SAME_AROUND;
    //    // set position check
    //    UInt32Point checkPos = pos;
    //    switch (i)
    //    {
    //    case 0: // top-left
    //        if (!checkPos.getX() || !checkPos.y)
    //            continue;
    //        --checkPos.x;
    //        --checkPos.y;
    //        curTileCheck = OTHER_TOP_LEFT;
    //        break;

    //    case 1: // top
    //        if (!checkPos.y)
    //            continue;
    //        --checkPos.y;
    //        curTileCheck = OTHER_TOP;
    //        break;

    //    case 2: // top-right
    //        if (!checkPos.y || checkPos.x+1 >= getSize().x)
    //            continue;
    //        ++checkPos.x;
    //        --checkPos.y;
    //        curTileCheck = OTHER_TOP_RIGHT;
    //        break;

    //    case 3: // left
    //        if (!checkPos.x)
    //            continue;
    //        --checkPos.x;
    //        curTileCheck = OTHER_LEFT;
    //        break;

    //    case 4: // right
    //        if (checkPos.x+1 >= getSize().x)
    //            continue;
    //        ++checkPos.x;
    //        curTileCheck = OTHER_RIGHT;
    //        break;

    //    case 5: // bottom-left
    //        if (!checkPos.x || checkPos.y+1 >= getSize().y)
    //            continue;
    //        --checkPos.x;
    //        ++checkPos.y;
    //        curTileCheck = OTHER_BOTTOM_LEFT;
    //        break;

    //    case 6: // bottom
    //        if (checkPos.y+1 >= getSize().y)
    //            continue;
    //        ++checkPos.y;
    //        curTileCheck = OTHER_BOTTOM;
    //        break;

    //    case 7: // bottom-right
    //        if (checkPos.x+1 >= getSize().x || checkPos.y+1 >= getSize().y)
    //            continue;
    //        ++checkPos.x;
    //        ++checkPos.y;
    //        curTileCheck = OTHER_BOTTOM_RIGHT;
    //        break;
    //    }
    //    auto mapTile = getMapTile(checkPos);
    //    // if bad object, continue
    //    if (!mapTile.getMapTile().isValid())
    //        continue;

    //    // if not same
    //    if (mapTile.getMapTile().m_uiAutoTileSetID != uiID)
    //    {
    //        uiBorderCheck += curTileCheck;
    //        if (resultFlag & FLAG_OTHER)
    //            result.push_back(checkPos);
    //    }
    //    // if same
    //    else if (resultFlag & FLAG_SAME)
    //        result.push_back(checkPos);
    //}
    return uiBorderCheck;
}

MapTileInfo Layer::getBorderTileInfo(const UInt32Point& pos, BorderTile borderTile) const
{
    UInt32Point checkPos = pos;
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

uint32 Layer::checkAutoTile(const UInt32Point& pos) const
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
