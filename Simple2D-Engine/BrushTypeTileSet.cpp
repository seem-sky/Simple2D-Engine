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
                    if (info.getPosition().x > getStartPosition().x)
                        diff.x = info.getPosition().x - getStartPosition().x;
                    else
                        diff.x = getStartPosition().x - info.getPosition().x;

                    if (info.getPosition().y > getStartPosition().y)
                        diff.y = info.getPosition().y - getStartPosition().y;
                    else
                        diff.y = getStartPosition().y - info.getPosition().y;
                    info.getMapTile().m_uiTileID = pTileSet->getTileID(UInt32Point(diff.x % size.x, diff.y % size.y));
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