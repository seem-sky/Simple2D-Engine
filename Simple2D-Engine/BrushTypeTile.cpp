#include "BrushTypeTile.h"
#include "MapException.h"
#include "DatabasePrototypes.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace TYPE
        {
            Tile::Tile(uint32 tileID, Layer& layer, const UInt32Point& pos) : Interface(tileID, layer, pos)
            {
            }

            void Tile::setTiles(const MapTileInfoVec& tiles)
            {
                for (auto info : tiles)
                {
                    try
                    {
                        info.getMapTile().m_uiAutoTileSetID = 0;
                        info.getMapTile().m_uiTileID = getTileID();
                        m_Layer.setMapTile(info);
                    }
                    catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
                }
            }

            void Tile::setBorderTiles(const MapTileInfoVec& tiles)
            {
                for (auto info : tiles)
                {
                    checkAutoTile(info);
                    try
                    {
                        m_Layer.setMapTile(info);
                    }
                    catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
                }
            }

            void Tile::checkAutoTile(MapTileInfo& info) const
            {
                uint32 check = 0;
                for (uint32 i = 0; i < 8; ++i)
                {
                    auto borderTile = static_cast<BorderTile>(i);
                    try
                    {
                        auto mapTileInfo = m_Layer.getBorderTileInfo(info.getPosition(), borderTile);
                        if (!mapTileInfo.isValid() || info.getMapTile().m_uiAutoTileSetID == mapTileInfo.getMapTile().m_uiAutoTileSetID)
                            continue;

                        check |= borderTileToTileCheck(borderTile);
                    }
                    catch (const EXCEPTION::TileOutOfRangeException&) {}
                }
                info.getMapTile().m_uiTileID = DATABASE::PROTOTYPE::AUTO_TILE::getAutoTileIndexForTileCheck(check);
            }
        }
    }
}
