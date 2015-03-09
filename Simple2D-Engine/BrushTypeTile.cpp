#include "BrushTypeTile.h"
#include "MapException.h"
#include "BrushRevert.h"
#include "MapLayer.h"

using namespace MAP::BRUSH;
using namespace TYPE;

Tile::Tile(uint32 tileID, const database::Manager& DBMgr, MAP::Layer& layer, const GEOMETRY::Point<uint32>& pos)
    : Type(tileID, DBMgr, layer, pos)
{}

void Tile::setTiles(const MAP::MapTileInfoVec& tiles, REVERT::BrushRevert& revert)
{
    for (auto info : tiles)
    {
        try
        {
            revert.addTile(info);
            info.getMapTile().m_uiAutoTileSetID = 0;
            info.getMapTile().m_uiTileID = getID();
            m_Layer.setMapTile(info);
        }
        catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
    }
}
