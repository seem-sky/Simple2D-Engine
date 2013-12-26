#ifndef REVERT_INFO_H
#define REVERT_INFO_H

#include "MapLayer.h"
#include <unordered_map>

namespace MAP
{
    namespace BRUSH
    {
        enum class BrushType
        {
            PEN,
            FILL
        };

        enum class SelectionType
        {
            TILES,
            AUTO_TILES,
            TILE_SETS
        };

        namespace REVERT
        {
            /*#####
            # RevertInfo
            #####*/
            // unordered_map typedefs
            typedef std::unordered_map<UInt32Point, MapTile> UInt32Point_MapTileUMap;
            class RevertInfo
            {
            public:
                void revert(MapLayer &mapLayer);

            public:
                UInt32Point_MapTileUMap m_Tiles;
                uint32 m_LayerIndex;
                Layer m_LayerType;
            };
        }
    }
}

#endif
