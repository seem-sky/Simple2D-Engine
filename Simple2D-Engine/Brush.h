#ifndef BRUSH_H
#define BRUSH_H

#include "MapLayer.h"
#include "DatabaseMgr.h"

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

        /*#####
        # BrushInfo
        #####*/
        class BrushInfo
        {
        public:
            BrushInfo() : m_BrushType(BrushType::PEN), m_SelectionType(SelectionType::TILES), m_ID(0)
            {
            }

            BrushType m_BrushType;
            SelectionType m_SelectionType;
            uint32 m_ID;
        };

        static bool operator==(const BrushInfo& lhs, const BrushInfo& rhs)
        {
            return lhs.m_ID == rhs.m_ID && lhs.m_SelectionType == rhs.m_SelectionType && lhs.m_BrushType == rhs.m_BrushType;
        }

        static bool operator!=(const BrushInfo& lhs, const BrushInfo& rhs)
        {
            return !(lhs == rhs);
        }

        /*#####
        # RevertInfo
        #####*/
        class RevertInfo
        {
        public:
            std::vector<std::pair<MapTile, UInt32Point>> m_Tiles;
            uint32 m_LayerIndex;
            Layer m_LayerType;
        };

        /*#####
        # Brush
        #####*/
        class Brush
        {
        protected:
            Brush(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, uint32 ID, SelectionType selectionType, Layer layerType, uint32 layerIndex);

        public:
            virtual void draw(const UInt32Point& pos) = 0;

            inline uint32 getID() const { return m_ID; }
            inline SelectionType getSelectionType() const { return m_SelectionType; }
            inline Layer getLayerType() const { return m_LayerType; }
            inline uint32 getLayerIndex() const { return m_LayerIndex; }

            const RevertInfo& getRevertInfo() const { return m_RevertInfo; }

        private:
            uint32 m_ID;
            SelectionType m_SelectionType;
            Layer m_LayerType;
            uint32 m_LayerIndex;

        protected:
            const DATABASE::DatabaseMgr& m_DBMgr;
            RevertInfo m_RevertInfo;
            MapLayer& m_MapLayer;
        };
        typedef std::unique_ptr<Brush> BrushPtr;

        class BrushPen : public Brush
        {
            friend class BrushFactory;

        private:
            void _updateAutoTilesAround(const UInt32PointVector& positions);
            void _drawTile(const UInt32Point& pos);
            void _drawAutoTile(const UInt32Point& pos);
            void _drawTileSet(const UInt32Point& pos);

        protected:
            BrushPen(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, uint32 ID, SelectionType selectionType, Layer layerType, uint32 layerIndex);

        public:
            void draw(const UInt32Point& pos);
        };

        class BrushFill : public Brush
        {
            friend class BrushFactory;
        protected:
            BrushFill(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, uint32 ID, SelectionType selectionType, Layer layerType, uint32 layerIndex);

        public:
            void draw(const UInt32Point& pos);
        };

        /*#####
        # BrushFactory
        #####*/
        class BrushFactory
        {
        public:
            static BrushPtr createBrush(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, const BrushInfo& info, Layer layerType, uint32 layerIndex);
        };
    }
}

#endif
