#ifndef REVERT_INFO_H
#define REVERT_INFO_H

#include <Bitset2D.h>
#include "MapTile.h"
#include "RevertInterface.h"

namespace MAP
{
    class LayerContainer;

    namespace BRUSH
    {
        namespace REVERT
        {
            /*#####
            # Revert
            #####*/
            class BrushRevert : public MAP::REVERT::Interface
            {
            public:
                BrushRevert(uint8 layerIndex, LayerType layerType, LayerContainer& layer);

                void revert();

                void addTile(const MapTileInfo& info);
                void addTiles(const MapTileInfoVec& tileInfos);

                inline bool hasChanges() const { return !m_Tiles.empty(); }

                void clear();

            private:
                MapTileInfoVec m_Tiles;
                Bitset2D m_Check;

                uint8 m_LayerIndex;
                LayerType m_LayerType;
                LayerContainer& m_MapLayer;
            };
        }
    }
}

#endif
