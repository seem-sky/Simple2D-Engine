#include "BrushTypeAutoTile.h"
#include "MapException.h"
#include "BrushRevert.h"
#include "MapLayer.h"

using namespace MAP::BRUSH;
using namespace TYPE;

AutoTile::AutoTile(uint32 tileID, const database::Manager& DBMgr, MAP::Layer& layer, const GEOMETRY::Point<uint32>& pos)
    : Type(tileID, DBMgr, layer, pos)
{}

void AutoTile::setTiles(const MAP::MapTileInfoVec& tiles, REVERT::BrushRevert& revert)
{
    for (auto info : tiles)
    {
        try
        {
            revert.addTile(info);
            info.getMapTile().m_uiAutoTileSetID = getID();
            info.getMapTile().m_uiTileID = DATABASE::PROTOTYPE::AUTO_TILE::INDEX_CENTER;
            m_Layer.setMapTile(info);
        }
        catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
    }
}
