#include "BrushAreaSingle.h"
#include "MapException.h"
#include "MapLayer.h"
#include <Bitset2D.h>
#include <algorithm>

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            Single::Single(Layer& layer, const GEOMETRY::Point<uint32>& pos) : Area(layer, pos)
            {
            }

            void Single::_do(const MapTileInfo& info, MapTileInfoVec& tiles, PointVec<uint32>& borderTiles)
            {
                // first set complete brush size -> bitset is checked
                sl::Bitset2D bitset;
                for (GEOMETRY::Point<uint32> pos; pos.getX() < getBrushSize().getWidth(); ++pos.getX())
                {
                    for (pos.getY() = 0; pos.getY() < getBrushSize().getHeight(); ++pos.getY())
                    {
                        try
                        {
                            auto mapTileInfo = m_Layer.getMapTile(pos + getStartPosition());
                            tiles.push_back(mapTileInfo);
                            if (pos.getX() == 0 || pos.getX() + 1 == getBrushSize().getWidth())
                            {
                                if (pos.getY() == 0 || pos.getY() + 1 == getBrushSize().getHeight())
                                    borderTiles.push_back(mapTileInfo.getPosition());
                            }

                            // resize bitset if needed
                            bitset.resize(std::max(bitset.getWidth(), pos.getX() + mapTileInfo.getPosition().getX() + 1),
                                std::max(bitset.getHeight(), pos.getY() + mapTileInfo.getPosition().getY() + 1));

                            bitset.set(pos + mapTileInfo.getPosition());
                        }
                        catch (const EXCEPTION::TileOutOfRangeException&) {}
                    }
                }

                // after that, we can really check borders
                for (GEOMETRY::Point<uint32> pos; pos.getX() < getBrushSize().getWidth(); ++pos.getX())
                {
                    for (pos.getY() = 0; pos.getY() < getBrushSize().getHeight(); ++pos.getY())
                        _storeBorder(getStartPosition() + pos, borderTiles, bitset);
                }
                
            }

            void Single::_storeBorder(const GEOMETRY::Point<uint32>& pos, PointVec<uint32>& borderTiles, sl::Bitset2D& bitset)
            {
                for (uint32 i = 0; i < 8; ++i)
                {
                    try
                    {
                        auto mapTileInfo = m_Layer.getBorderTileInfo(pos, static_cast<BorderTile>(i));
                        // resize bitset if needed
                        bitset.resize(std::max(bitset.getWidth(), pos.getX() + mapTileInfo.getPosition().getX() + 1),
                            std::max(bitset.getHeight(), pos.getY() + mapTileInfo.getPosition().getY() + 1));
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
