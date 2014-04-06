#ifndef BRUSH_H
#define BRUSH_H

#include "DatabaseMgr.h"
#include "RevertInfo.h"

namespace MAP
{
    namespace BRUSH
    {
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
        # Brush
        #####*/
        class Brush
        {
        protected:
            Brush(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, uint32 ID, SelectionType selectionType, LayerType layerType, uint32 layerIndex);

            void updateAutoTilesAround(const UInt32PointVector& positions);

        public:
            virtual void draw(const UInt32Point& pos) = 0;

            inline uint32 getID() const { return m_ID; }
            inline SelectionType getSelectionType() const { return m_SelectionType; }
            inline LayerType getLayerType() const { return m_LayerType; }
            inline uint32 getLayerIndex() const { return m_LayerIndex; }

            const REVERT::RevertInfo& getRevertInfo() const { return m_RevertInfo; }

        private:
            uint32 m_ID;
            SelectionType m_SelectionType;
            LayerType m_LayerType;
            uint32 m_LayerIndex;

        protected:
            const DATABASE::DatabaseMgr& m_DBMgr;
            REVERT::RevertInfo m_RevertInfo;
            LayerContainer& m_MapLayer;
        };
        typedef std::unique_ptr<Brush> BrushPtr;

        class BrushPen : public Brush
        {
            friend class BrushFactory;

        private:
            void _drawTile(const UInt32Point& pos);
            void _drawAutoTile(const UInt32Point& pos);
            void _drawTileSet(const UInt32Point& pos);

        protected:
            BrushPen(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, uint32 ID, SelectionType selectionType, LayerType layerType, uint32 layerIndex);

        public:
            void draw(const UInt32Point& pos);
        };

        class BrushFill : public Brush
        {
            friend class BrushFactory;

        private:
            void _drawTile(const UInt32Point& pos);
            void _drawAutoTile(const UInt32Point& pos);
            void _drawTileSet(const UInt32Point& pos);
            
            bool _getPosition(uint32 i, UInt32Point3D &checkPos) const;

        protected:
            BrushFill(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, uint32 ID, SelectionType selectionType, LayerType layerType, uint32 layerIndex);

        public:
            void draw(const UInt32Point& pos);
        };

        /*#####
        # BrushFactory
        #####*/
        class BrushFactory
        {
        public:
            static BrushPtr createBrush(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, const BrushInfo& info, LayerType layerType, uint32 layerIndex);
        };
    }
}

#endif
