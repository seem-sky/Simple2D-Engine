#include "BrushAreaSingle.h"
#include "MapException.h"
#include "MapLayer.h"
#include <Bitset2D.h>

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
                // first set complete brush size -> bitset is checked
                Bitset2D bitset;
                for (UInt32Point pos; pos.getX() < getBrushSize().getX(); ++pos.getX())
                {
                    for (pos.getY() = 0; pos.getY() < getBrushSize().getY(); ++pos.getY())
                    {
                        try
                        {
                            auto mapTileInfo = m_Layer.getMapTile(pos + getStartPosition());
                            tiles.push_back(mapTileInfo);
                            if (pos.getX() == 0 || pos.getX() + 1 == getBrushSize().getX())
                            {
                                if (pos.getY() == 0 || pos.getY() + 1 == getBrushSize().getY())
                                    borderTiles.push_back(mapTileInfo.getPosition());
                            }
                            bitset.set(pos + mapTileInfo.getPosition());
                        }
                        catch (const EXCEPTION::TileOutOfRangeException&) {}
                    }
                }

                // after that, we can really check borders
                for (UInt32Point pos; pos.getX() < getBrushSize().getX(); ++pos.getX())
                {
                    for (pos.getY() = 0; pos.getY() < getBrushSize().getY(); ++pos.getY())
                        _storeBorder(getStartPosition() + pos, borderTiles, bitset);
                }
                
            }

            void Single::_storeBorder(const UInt32Point& pos, UInt32PointVec& borderTiles, Bitset2D& bitset)
            {
                for (uint32 i = 0; i < 8; ++i)
                {
                    try
                    {
                        auto mapTileInfo = m_Layer.getBorderTileInfo(pos, static_cast<BorderTile>(i));
                        if (bitset.get(mapTileInfo.getPosition()))
                            continue;
                        bitset.set(mapTileInfo.getPosition());

                        if (mapTileInfo.isValid() && mapTileInfo.isAutoTile())
                            borderTiles.push_back(mapTileInfo.getPosition());
                    }
                    catch (const EXCEPTION::TileOutOfRangeException&) {}
                }
            }
        }
    }
}
