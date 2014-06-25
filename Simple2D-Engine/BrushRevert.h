#ifndef REVERT_INFO_H
#define REVERT_INFO_H

#include <Bitset2D.h>
#include "MapTile.h"
#include "BrushRevertInfo.h"

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
            class BrushRevert
            {
            public:
                void revert(LayerContainer& mapLayer);

                void addTile(const MapTileInfo& info);
                void addTiles(const MapTileInfoVec& tileInfos);

                inline bool hasChanges() const { return !m_Tiles.empty(); }

                void setBrushRevertInfo(const BrushRevertInfo& info) { m_Info = info; }
                BrushRevertInfo getBrushRevertInfo() const { return m_Info; }

                void clear();

            private:
                MapTileInfoVec m_Tiles;
                Bitset2D m_Check;

                BrushRevertInfo m_Info;
            };
        }
    }
}

#endif
