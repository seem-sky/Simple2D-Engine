#include "BrushArea.h"
#include "DatabasePrototypes.h"

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
                auto info = m_Layer.getMapTile(getStartPosition());
                m_OldTile = info.getMapTile();
                try
                {
                    _do(info, tiles, borderTiles);
                }
                catch (const MAP::EXCEPTION::TileOutOfRangeException&) {}
            }
        }
    }
}