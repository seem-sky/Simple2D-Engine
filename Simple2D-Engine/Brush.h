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
        # Brush
        #####*/
        class Brush
        {
        protected:
            Brush(const DATABASE::DatabaseMgr& DBMgr, uint32 ID, SelectionType selectionType);

        public:
            virtual void draw(MapLayer& mapLayer, const UInt32Point3D& pos, Layer layerType) const = 0;

            inline uint32 getID() const { return m_ID; }
            inline SelectionType getSelectionType() const { return m_SelectionType; }

        private:
            uint32 m_ID;
            SelectionType m_SelectionType;

        protected:
            const DATABASE::DatabaseMgr& m_DBMgr;
        };
        typedef std::unique_ptr<Brush> BrushPtr;

        class BrushPen : public Brush
        {
            friend class BrushFactory;

        private:
            void _updateAutoTilesAround(MapLayer& mapLayer, const UInt32PointVector& positions, uint32 layerIndex, Layer layerType) const;
            void _drawTile( MapLayer &mapLayer, const UInt32Point3D &pos, Layer layerType ) const;
            void _drawAutoTile( MapLayer &mapLayer, const UInt32Point3D &pos, Layer layerType ) const;
            void _drawTileSet( const UInt32Point3D &pos, MapLayer &mapLayer, Layer layerType ) const;

        protected:
            BrushPen(const DATABASE::DatabaseMgr& DBMgr, uint32 ID, SelectionType selectionType);

        public:
            void draw(MapLayer& mapLayer, const UInt32Point3D& pos, Layer layerType) const;
        };

        class BrushFill : public Brush
        {
            friend class BrushFactory;
        protected:
            BrushFill(const DATABASE::DatabaseMgr& DBMgr, uint32 ID, SelectionType selectionType);

        public:
            void draw(MapLayer& mapLayer, const UInt32Point3D& pos, Layer layerType) const;
        };

        /*#####
        # BrushFactory
        #####*/
        class BrushFactory
        {
        public:
            static BrushPtr createBrush(const DATABASE::DatabaseMgr& DBMgr, const BrushInfo& info);
        };
    }
}

#endif
