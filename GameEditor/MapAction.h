#ifndef MAP_ACTION_H
#define MAP_ACTION_H

#include "MapDatabase.h"

namespace DATABASE
{
    namespace MAP_STRUCTURE
    {
        /*#####
        # MapAction
        #####*/
        class MapAction
        {
        public:
            MapAction(MapPrototypePtr map);

            virtual void revertMapAction() = 0;

            inline MapPrototypePtr getMap() const {return m_pMap; } 

        private:
            MapPrototypePtr m_pMap;
        };
        typedef std::shared_ptr<MapAction> MapActionPtr;
        typedef std::vector<MapActionPtr> MapActionPtrVector;

        /*#####
        # TileMapAction
        #####*/
        class TileMapAction : public MapAction
        {
        public:
            TileMapAction(UInt32Point3D pos, MapTile uiTileID, Layer layer, MapPrototypePtr map);

            void revertMapAction();

        private:
            UInt32Point3D m_Pos;
            MapTile m_MapTile;
            Layer m_Layer;
        };

        class MultiPositionTileMapAction : public MapAction
        {
        public:
            MultiPositionTileMapAction(MapTile mapTile, MapPrototypePtr map, uint32 uiLayer, Layer layer);

            void revertMapAction();
            void addPosition(UInt32Point pos);

        private:
            UInt32PointVector m_Positions;
            uint32 m_uiLayer;
            MapTile m_MapTile;
            Layer m_Layer;
        };
        typedef std::shared_ptr<MultiPositionTileMapAction> MultiPositionTileMapActionPtr;

        typedef std::pair<UInt32Point, MapTile> UInt32PointMapTilePair;
        typedef std::vector<UInt32PointMapTilePair> UInt32PointMapTilePairVector;
        class MultiPositionMultiTileMapAction : public MapAction
        {
        private:
            void _revertAction(UInt32PointMapTilePair pair);

        public:
            MultiPositionMultiTileMapAction(MapPrototypePtr map, uint32 uiLayer, Layer layer);

            void revertMapAction();
            void addPosition(UInt32Point pos, MapTile tile);

        private:
            UInt32PointMapTilePairVector m_Positions;
            uint32 m_uiLayer;
            Layer m_Layer;
        };
        typedef std::shared_ptr<MultiPositionMultiTileMapAction> MultiPositionMultiTileMapActionPtr;
    }
}
#endif