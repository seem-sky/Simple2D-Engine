#include "BrushAreaFill.h"
#include "MapException.h"
#include "MapLayer.h"
#include <log/Log.h>
#include <algorithm>

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            Fill::Fill(Layer& layer, const GEOMETRY::Point<uint32>& pos) : Area(layer, pos)
            {
            }

            void Fill::_do(const MapTileInfo& info, MapTileInfoVec& tiles, PointVec<uint32>& borderTiles)
            {
                if (getBrushSize() != GEOMETRY::Point<uint32>(1, 1))
                    WARNING_MESSAGE("Brush size other then 1/1 has no effect in fill mode.");

                tiles.push_back(info);
                Bitset2D tileBitset, borderBitset;
                // resize bitset if needed
                tileBitset.resize(std::max(tileBitset.width(), getStartPosition().getX() + 1),
                    std::max(tileBitset.height(), getStartPosition().getY() + 1));
                tileBitset.set(getStartPosition());

                PointVec<uint32> openPositions{ getStartPosition() };
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

            void Fill::_checkTile(const GEOMETRY::Point<uint32>& pos, Bitset2D& bitset, Bitset2D& borderBitset,
                PointVec<uint32>& openPositions, MapTileInfoVec& tileInfos, PointVec<uint32>& borderTiles)
            {
                auto mapTileInfo = m_Layer.getMapTile(pos);
                bool check = false;
                for (uint32 i = 0; i < 8; ++i)
                {
                    try
                    {
                        auto borderTile = static_cast<BorderTile>(i);
                        auto mapTileInfo = m_Layer.getBorderTileInfo(pos, borderTile);
                        // resize bitset if needed
                        bitset.resize(std::max(bitset.width(), mapTileInfo.getPosition().getX() + 1),
                            std::max(bitset.height(), mapTileInfo.getPosition().getY() + 1));

                        if (!mapTileInfo.isValid() || bitset.get(mapTileInfo.getPosition()))
                            continue;

                        if (getOldMapTile().isAutoTile() != mapTileInfo.isAutoTile() ||
                            getOldMapTile().isAutoTile() && mapTileInfo.isAutoTile() && getOldMapTile().m_uiAutoTileSetID != mapTileInfo.getMapTile().m_uiAutoTileSetID ||
                            !getOldMapTile().isAutoTile() && !mapTileInfo.isAutoTile() && getOldMapTile().m_uiTileID != mapTileInfo.getMapTile().m_uiTileID)
                        {
                            check = true;
                            // resize bitset if needed
                            borderBitset.resize(std::max(borderBitset.width(), mapTileInfo.getPosition().getX() + 1),
                                std::max(borderBitset.height(), mapTileInfo.getPosition().getY() + 1));
                            if (mapTileInfo.isAutoTile() && !borderBitset.get(mapTileInfo.getPosition()))
                            {
                                borderBitset.set(mapTileInfo.getPosition());
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
                if (check)
                {
                    // resize bitset if needed
                    borderBitset.resize(std::max(borderBitset.width(), mapTileInfo.getPosition().getX() + 1),
                        std::max(borderBitset.height(), mapTileInfo.getPosition().getY() + 1));
                    if (!borderBitset.get(mapTileInfo.getPosition()))
                    {
                        borderBitset.set(mapTileInfo.getPosition());
                        borderTiles.push_back(mapTileInfo.getPosition());
                    }
                }
            }
        }
    }
}
