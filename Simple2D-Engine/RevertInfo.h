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
            typedef std::vector<std::pair<UInt32Point, MapTile>> UInt32Point_MapTileVector;
            class RevertInfo
            {
            public:
                RevertInfo(const UInt32Point& layerSize, uint32 layerIndex, Layer layerType);

                void revert(MapLayer &mapLayer);

                void addTile(const UInt32Point& pos, const MapTile& tile);

            private:
                UInt32Point_MapTileVector m_Tiles;
                uint32 m_LayerIndex;
                Layer m_LayerType;

                boost::dynamic_bitset<> m_Check;
                const UInt32Point m_LayerSize;
            };
        }
    }
}

#endif
