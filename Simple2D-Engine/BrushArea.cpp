#include "BrushArea.h"
#include "DatabasePrototypes.h"
#include "MapException.h"
#include "MapLayer.h"

namespace MAP
{
    namespace BRUSH
    {
        namespace AREA
        {
            void Area::start(MapTileInfoVec& tiles, PointVec<uint32>& borderTiles)
            {
                tiles.clear();
                borderTiles.clear();
                try
                {
                    auto info = m_Layer.getMapTile(getStartPosition());
                    m_OldTile = info.getMapTile();
                    _do(info, tiles, borderTiles);
                }
                catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
            }

            void Area::setBrushSize(const GEOMETRY::Point<uint32>& size)
            {
                m_BrushSize = size;
            }

            GEOMETRY::Point<uint32> Area::getBrushSize() const
            {
                return m_BrushSize;
            }
        }
    }
}