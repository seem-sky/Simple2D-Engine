#include "BrushTypeAutoTile.h"
#include "MapException.h"
#include "DatabasePrototypes.h"
#include "BrushRevert.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            AutoTile::AutoTile(uint32 tileID, Layer& layer, const UInt32Point& pos) : Type(tileID, layer, pos)
            {
            }

            void AutoTile::setTiles(const MapTileInfoVec& tiles, REVERT::BrushRevert& revert)
            {
                for (auto info : tiles)
                {
                    try
                    {
                        revert.addTile(info);
                        info.getMapTile().m_uiAutoTileSetID = getTileID();
                        info.getMapTile().m_uiTileID = DATABASE::PROTOTYPE::AUTO_TILE::INDEX_CENTER;
                        m_Layer.setMapTile(info);
                    }
                    catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
                }
            }
        }
    }
}
