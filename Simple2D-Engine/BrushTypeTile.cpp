#include "BrushTypeTile.h"
#include "MapException.h"
#include "BrushRevert.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            Tile::Tile(uint32 tileID, Layer& layer, const UInt32Point& pos) : Type(tileID, layer, pos)
            {
            }

            void Tile::setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert)
            {
                for (auto info : tiles)
                {
                    try
                    {
                        revert.addTile(info);
                        info.getMapTile().m_uiAutoTileSetID = 0;
                        info.getMapTile().m_uiTileID = getTileID();
                        m_Layer.setMapTile(info);
                    }
                    catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
                }
            }
        }
    }
}
