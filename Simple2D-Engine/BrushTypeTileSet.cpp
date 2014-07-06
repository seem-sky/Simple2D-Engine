#include "BrushTypeTileSet.h"
#include "DatabaseMgr.h"
#include "MapException.h"
#include "BrushRevert.h"
#include "MapLayer.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            TileSet::TileSet(uint32 tileID, const DATABASE::DatabaseMgr& DBMgr, Layer& layer, const UInt32Point& pos) : Type(tileID, DBMgr, layer, pos)
            {}

            void TileSet::setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert)
            {
                auto pTileSet = m_DBMgr.getTileSetDatabase()->getOriginalPrototype(getID());
                if (!pTileSet)
                    return;

                auto size = pTileSet->getTileSetSize();
                for (auto info : tiles)
                {
                    revert.addTile(info);
                    info.getMapTile().m_uiAutoTileSetID = 0;
                    UInt32Point diff;
                    if (info.getPosition().getX() > getStartPosition().getX())
                        diff.getX() = info.getPosition().getX() - getStartPosition().getX();
                    else
                        diff.getX() = getStartPosition().getX() - info.getPosition().getX();

                    if (info.getPosition().getY() > getStartPosition().getY())
                        diff.getY() = info.getPosition().getY() - getStartPosition().getY();
                    else
                        diff.getY() = getStartPosition().getY() - info.getPosition().getY();
                    info.getMapTile().m_uiTileID = pTileSet->getTileID(UInt32Point(diff.getX() % size.getX(), diff.getY() % size.getY()));
                    try
                    {
                        m_Layer.setMapTile(info);
                    }
                    catch (const EXCEPTION::TileOutOfRangeException&)
                    { }
                }
            }
        }
    }
}