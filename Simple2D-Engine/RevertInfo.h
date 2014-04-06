#ifndef REVERT_INFO_H
#define REVERT_INFO_H

#include "MapLayer.h"
#include <Bitset2D.h>

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
            class RevertInfo
            {
            public:
                RevertInfo(LayerType type, uint8 index);

                void revert(LayerContainer &mapLayer);
                void addTile(const UInt32Point& pos, const MapTile& tile);
                inline bool hasChanges() const { return !m_Tiles.empty(); }

            private:
                typedef std::vector<std::pair<UInt32Point, MapTile>> UInt32Point_MapTileVector;
                UInt32Point_MapTileVector m_Tiles;

                Bitset2D m_Check;

                LayerType m_LayerType;
                uint8 m_LayerIndex;
            };
        }
    }
}

#endif
