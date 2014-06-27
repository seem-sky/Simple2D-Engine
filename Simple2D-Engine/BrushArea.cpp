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
            void Area::start(MapTileInfoVec& tiles, UInt32PointVec& borderTiles)
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

            void Area::setBrushSize(const UInt32Point& size)
            {
                m_BrushSize = size;
            }

            UInt32Point Area::getBrushSize() const
            {
                return m_BrushSize;
            }
        }
    }
}