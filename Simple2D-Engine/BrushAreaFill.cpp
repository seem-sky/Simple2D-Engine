#include "BrushAreaFill.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            Fill::Fill(Layer& layer, const UInt32Point& pos) : Area(layer, pos)
            {
            }

            void Fill::_do(const MapTileInfo& info, MapTileInfoVec& tiles, UInt32PointVec& borderTiles)
            {
                tiles.push_back(info);
                Bitset2D tileBitset, borderBitset;
                tileBitset.set(getStartPosition());

                UInt32PointVec openPositions{ getStartPosition() };
                while (!openPositions.empty())
                {
                    auto position = openPositions.back();
                    openPositions.pop_back();
                    try
                    {
                        _checkTile(position, tileBitset, borderBitset, openPositions, tiles, borderTiles);
                    }
                    catch (const EXCEPTION::TileOutOfRangeException&) {}
                }
            }

            void Fill::_checkTile(const UInt32Point& pos, Bitset2D& bitset, Bitset2D& borderBitset,
                UInt32PointVec& openPositions, MapTileInfoVec& tileInfos, UInt32PointVec& borderTiles)
            {
                auto mapTileInfo = m_Layer.getMapTile(pos);
                bool check = false;
                for (uint32 i = 0; i < 8; ++i)
                {
                    try
                    {
                        auto borderTile = static_cast<BorderTile>(i);
                        auto mapTileInfo = m_Layer.getBorderTileInfo(pos, borderTile);
                        if (!mapTileInfo.isValid() || bitset.get(mapTileInfo.getPosition()))
                            continue;

                        if (getOldMapTile().isAutoTile() != mapTileInfo.isAutoTile() ||
                            getOldMapTile().isAutoTile() && mapTileInfo.isAutoTile() && getOldMapTile().m_uiAutoTileSetID != mapTileInfo.getMapTile().m_uiAutoTileSetID ||
                            !getOldMapTile().isAutoTile() && !mapTileInfo.isAutoTile() && getOldMapTile().m_uiTileID != mapTileInfo.getMapTile().m_uiTileID)
                        {
                            check = true;
                            if (!borderBitset.get(mapTileInfo.getPosition()))
                            {
                                borderBitset.set(mapTileInfo.getPosition());
                                if (mapTileInfo.isAutoTile())
                                    borderTiles.push_back(mapTileInfo.getPosition());
                            }
                        }
                        else  if (borderTile == MAP::BorderTile::LEFT || borderTile == MAP::BorderTile::RIGHT || borderTile == MAP::BorderTile::BOTTOM ||
                            borderTile == MAP::BorderTile::TOP)
                        {
                            bitset.set(mapTileInfo.getPosition());
                            openPositions.push_back(mapTileInfo.getPosition());
                            tileInfos.push_back(mapTileInfo);
                        }
                    }
                    catch (const EXCEPTION::TileOutOfRangeException&) {}
                }

                // push if border tile
                if (check && !borderBitset.get(mapTileInfo.getPosition()))
                {
                    borderBitset.set(mapTileInfo.getPosition());
                    borderTiles.push_back(mapTileInfo.getPosition());
                }
            }
        }
    }
}
