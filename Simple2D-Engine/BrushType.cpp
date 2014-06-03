#include "BrushType.h"

using namespace MAP;
using namespace BRUSH;

BrushType2::BrushType2(Layer& mapLayer, const MapTile& destTile) : m_MapLayer(mapLayer), m_DestTile(destTile)
{
}

void BrushType2::setTile(const UInt32Point& at)
{
    auto oldTile = m_MapLayer.getMapTile(at);
    // update autotiles
    if (oldTile.isAutoTile() || m_DestTile.isAutoTile())
    {
        m_MapLayer.setMapTile(at, MapTile(oldTile.m_uiTileID, m_DestTile.m_uiAutoTileSetID));
        
    }
}


//for (uint32 i = 0; i < 8; ++i)
//{
//    DATABASE::AUTO_TILE::TILE_CHECK curTileCheck = DATABASE::AUTO_TILE::SAME_AROUND;
//    // set position check
//    UInt32Point3D checkPos = at;
//    switch (i)
//    {
//    case 0: // top-left
//        if (!checkPos.x || !checkPos.y)
//            continue;
//        --checkPos.x;
//        --checkPos.y;
//        curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP_LEFT;
//        break;
//
//    case 1: // top
//        if (!checkPos.y)
//            continue;
//        --checkPos.y;
//        curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP;
//        break;
//
//    case 2: // top-right
//        if (!checkPos.y || checkPos.x+1 >= m_MapLayer.getSize().x)
//            continue;
//        ++checkPos.x;
//        --checkPos.y;
//        curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP_RIGHT;
//        break;
//
//    case 3: // left
//        if (!checkPos.x)
//            continue;
//        --checkPos.x;
//        curTileCheck = DATABASE::AUTO_TILE::OTHER_LEFT;
//        break;
//
//    case 4: // right
//        if (checkPos.x+1 >= m_MapLayer.getSize().x)
//            continue;
//        ++checkPos.x;
//        curTileCheck = DATABASE::AUTO_TILE::OTHER_RIGHT;
//        break;
//
//    case 5: // bottom-left
//        if (!checkPos.x || checkPos.y+1 >= m_MapLayer.getSize().y)
//            continue;
//        --checkPos.x;
//        ++checkPos.y;
//        curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM_LEFT;
//        break;
//
//    case 6: // bottom
//        if (checkPos.y+1 >= m_MapLayer.getSize().y)
//            continue;
//        ++checkPos.y;
//        curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM;
//        break;
//
//    case 7: // bottom-right
//        if (checkPos.x+1 >= m_MapLayer.getSize().x || checkPos.y+1 >= m_MapLayer.getSize().y)
//            continue;
//        ++checkPos.x;
//        ++checkPos.y;
//        curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM_RIGHT;
//        break;
//    }
//    auto& mapTile = m_MapLayer.getMapTile(checkPos);
//    // if bad object, continue
//    if (!mapTile.isValid())
//        continue;
//    // update this tile