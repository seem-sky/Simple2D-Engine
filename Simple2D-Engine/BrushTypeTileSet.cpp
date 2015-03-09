#include "BrushTypeTileSet.h"
#include <Database/Manager.h>
#include "MapException.h"
#include "BrushRevert.h"
#include "MapLayer.h"

using namespace MAP::BRUSH;
using namespace TYPE;

TileSet::TileSet(uint32 tileID, const database::Manager& DBMgr, MAP::Layer& layer, const GEOMETRY::Point<uint32>& pos)
    : Type(tileID, DBMgr, layer, pos)
{}

void TileSet::setTiles(const MAP::MapTileInfoVec& tiles, REVERT::BrushRevert& revert)
{
    auto pTileSet = m_DBMgr.getTileSetDatabase().getPrototype(getID());
    if (!pTileSet)
        return;

    auto size = pTileSet->getSize();
    for (auto info : tiles)
    {
        revert.addTile(info);
        info.getMapTile().m_uiAutoTileSetID = 0;
        GEOMETRY::Point<uint32> diff;
        if (info.getPosition().getX() > getStartPosition().getX())
            diff.getX() = info.getPosition().getX() - getStartPosition().getX();
        else
            diff.getX() = getStartPosition().getX() - info.getPosition().getX();

        if (info.getPosition().getY() > getStartPosition().getY())
            diff.getY() = info.getPosition().getY() - getStartPosition().getY();
        else
            diff.getY() = getStartPosition().getY() - info.getPosition().getY();
        info.getMapTile().m_uiTileID = pTileSet->getTileID(GEOMETRY::Point<uint32>(diff.getX() % size.getWidth(), diff.getY() % size.getHeight()));
        try
        {
            m_Layer.setMapTile(info);
        }
        catch (const EXCEPTION::TileOutOfRangeException&)
        { }
    }
}
