#include "BrushAreaSingle.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            Single::Single(Layer& layer, const UInt32Point& pos) : Area(layer, pos)
            {
            }

            void Single::_do(const MapTileInfo& info, MapTileInfoVec& tiles, UInt32PointVec& borderTiles)
            {
                tiles.push_back(info);
                borderTiles.push_back(info.getPosition());
                _storeBorder(getStartPosition(), borderTiles);
            }

            void Single::_storeBorder(const UInt32Point& pos, UInt32PointVec& borderTiles)
            {
                for (uint32 i = 0; i < 8; ++i)
                {
                    try
                    {
                        auto mapTileInfo = m_Layer.getBorderTileInfo(pos, static_cast<BorderTile>(i));
                        if (mapTileInfo.isValid() && mapTileInfo.isAutoTile())
                            borderTiles.push_back(mapTileInfo.getPosition());
                    }
                    catch (const EXCEPTION::TileOutOfRangeException&) {}
                }
            }
        }
    }
}