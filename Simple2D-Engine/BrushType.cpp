#include "BrushType.h"
#include "MapException.h"
#include "DatabasePrototypes.h"
#include "BrushRevert.h"
#include "MapLayer.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            Type::Type(uint32 tileID, const DATABASE::DatabaseMgr& DBMgr, Layer& layer, const GEOMETRY::Point<uint32>& pos)
                : m_Layer(layer), m_StartPosition(pos), m_DBMgr(DBMgr), m_TileID(tileID)
            {}

            void Type::setBorderTiles(const PointVec<uint32>& tiles, REVERT::BrushRevert& revert)
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
