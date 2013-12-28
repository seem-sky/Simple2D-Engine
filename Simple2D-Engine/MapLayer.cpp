#include "MapLayer.h"
#include "DatabasePrototypes.h"

using namespace MAP;

void MapLayer::clear()
{
    resize(UInt32Point(), 0, 0);
}

void MapLayer::resize(const UInt32Point& size, uint8 uiForegroundLayerSize, uint8 uiBackgroundLayerSize)
{
    m_Size = size;
    m_uiLayer.at(static_cast<uint32>(Layer::LAYER_FOREGROUND)) = uiForegroundLayerSize;
    m_uiLayer.at(static_cast<uint32>(Layer::LAYER_BACKGROUND)) = uiBackgroundLayerSize;

    m_BackgroundTiles.resize(boost::extents[m_Size.x][m_Size.y][m_uiLayer.at(static_cast<uint32>(Layer::LAYER_BACKGROUND))]);
    m_ForegroundTiles.resize(boost::extents[m_Size.x][m_Size.y][m_uiLayer.at(static_cast<uint32>(Layer::LAYER_FOREGROUND))]);
}

const MapTile& MapLayer::getMapTile(const UInt32Point3D& at, Layer layer) const
{
    if (isInMap(at) && getLayerSize(layer) > at.z)
    {
        switch (layer)
        {
        case Layer::LAYER_BACKGROUND: return m_BackgroundTiles[at.x][at.y][at.z];
        case Layer::LAYER_FOREGROUND: return m_ForegroundTiles[at.x][at.y][at.z];
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
        case Layer::LAYER_BACKGROUND:
            m_BackgroundTiles[at.x][at.y][at.z] = tile;
            break;
        case Layer::LAYER_FOREGROUND:
            m_ForegroundTiles[at.x][at.y][at.z] = tile;
            break;
        }
    }
    else
        throw std::out_of_range(nullptr);
}

uint32 MapLayer::checkAutoTiles(uint32 uiID, const UInt32Point3D& pos, UInt32PointVector& result, Layer layer, uint32 resultFlag)
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
