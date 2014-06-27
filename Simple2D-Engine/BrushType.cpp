#include "BrushType.h"
#include "MapException.h"
#include "DatabasePrototypes.h"
#include "BrushRevert.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            void Type::setBorderTiles(const UInt32PointVec& tiles, REVERT::BrushRevert& revert)
{
                for (auto pos : tiles)
                {
                    try
                    {
                        auto newTileInfo = m_Layer.getMapTile(pos);
                        revert.addTile(newTileInfo);
                        newTileInfo.getMapTile().m_uiTileID = DATABASE::PROTOTYPE::AUTO_TILE::getAutoTileIndexForTileCheck(m_Layer.checkAutoTile(pos));
                        m_Layer.setMapTile(newTileInfo);
                    }
                    catch (const MAP::EXCEPTION::TileException&) {}
                }
            }
        }
    }
}
