#include "BrushAreaSingle.h"
#include "MapException.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            Single::Single(Layer& layer, const UInt32Point& pos) : Interface(layer, pos)
            {
            }

            void Single::start(MapTileInfoVec& tiles, MapTileInfoVec& borderTiles)
            {
                tiles.clear();
                borderTiles.clear();
                Bitset2D bitset;
                try
                {
                    auto info = m_Layer.getMapTile(getStartPosition());
                    m_OldTile = info.getMapTile();
                    tiles.push_back(info);
                    bitset.set(getStartPosition());
                    storeBorder(getStartPosition(), borderTiles, bitset);
                }
                catch (const MAP::EXCEPTION::TileRangeException&) {}
            }

            void Single::storeBorder(const UInt32Point& pos, MapTileInfoVec& borderTiles, Bitset2D& bitset)
            {
                for (uint32 i = 0; i < 8; ++i)
                {
                    try
                    {
                        auto mapTileInfo = m_Layer.getBorderTileInfo(pos, static_cast<BorderTile>(i));
                        if (bitset.get(mapTileInfo.getPosition()))
                            continue;

                        if (mapTileInfo.isValid() && mapTileInfo.isAutoTile() &&
                            mapTileInfo.getMapTile().m_uiAutoTileSetID == m_OldTile.m_uiAutoTileSetID)
                            borderTiles.push_back(mapTileInfo);
                        bitset.set(mapTileInfo.getPosition());
                    }
                    catch (const EXCEPTION::TileRangeException&)
                    {
                    }
                }
            }
        }
    }
}