#ifndef MAP_BRUSH_H
#define MAP_BRUSH_H

#include <Global.h>
#include "MapView.h"
#include <boost/dynamic_bitset.hpp>

namespace DATABASE
{
    namespace MAP_STRUCTURE
    {
        namespace BRUSH
        {
            typedef std::vector<boost::dynamic_bitset<>> BitsetVector;
            /*#####
            # TileBrushes
            #####*/
            enum BrushMode
            {
                MODE_PEN,
                MODE_RECT,
                MODE_FILL,
            };

            BrushMode getBrushMode(const QString &mode);

            enum BrushType
            {
                BRUSH_TILE,
                BRUSH_TILE_SET,
                BRUSH_AUTO_TILE
            };

            class TileBrush
            {
            public:


            private:
                virtual void _drawPen(MapViewer *pWidget, const UInt32Point3D &center) = 0;
                virtual void _drawFill(MapViewer *pWidget, const UInt32Point3D &center) = 0;
                virtual bool _checkFill(MapPrototypePtr map, MapTile centerTile, UInt32Point3D pos, BitsetVector &mapBitset, UInt32PointVector &openPoints) = 0;

            protected:
                void calculateFillArea(MapViewer *pWidget, const UInt32Point3D &center, BitsetVector &mapBitset = BitsetVector());
                void setupBitset(const MAP_STRUCTURE::MapPrototypePtr &map, BitsetVector &mapBitset);

            public:
                TileBrush();
                inline void setBrushMode(BrushMode type) { m_mode = type; }
                inline BrushMode getBrushMode() const { return m_mode; }
                inline void setTile(uint32 uiID) { m_uiTileID = uiID; }
                inline uint32 getTile() const { return m_uiTileID; }

                inline void setLayer(Layer layer) { m_Layer = layer; }
                inline Layer getLayer() const { return m_Layer; }

                void drawOnMap(MapViewer *pWidget, UInt32Point3D center);

                void brushPress(MapViewer *pWidget, UInt32Point3D center);
                void brushRelease(MapViewer *pWidget, UInt32Point3D center);
                void brushMove(MapViewer *pWidget, UInt32Point3D center);

                inline BrushType getBrushType() const { return m_BrushType; }

            private:
                bool m_buttonHold;
                MapViewer *m_pLastEmitter;
                BrushMode m_mode;
                uint32 m_uiTileID;
                Layer m_Layer;

            protected:
                BrushType m_BrushType;
            };

            class MapTileBrush : public TileBrush
            {
            private:
                void _drawPen(MapViewer *pWidget, const UInt32Point3D &center);
                void _drawFill(MapViewer *pWidget, const UInt32Point3D &center);
                bool _checkFill(MapPrototypePtr map, MapTile centerTile, UInt32Point3D pos, BitsetVector &mapBitset, UInt32PointVector &openPoints);

            public:
                MapTileBrush() : TileBrush()
                {
                    m_BrushType = BRUSH_TILE;
                }
            };

            class MapTileSetBrush : public TileBrush
            {
            private:
                void _drawPen(MapViewer *pWidget, const UInt32Point3D &center);
                void _drawFill(MapViewer *pWidget, const UInt32Point3D &center);
                bool _checkFill(MapPrototypePtr map, MapTile centerTile, UInt32Point3D pos, BitsetVector &mapBitset, UInt32PointVector &openPoints);

            public:
                MapTileSetBrush(DATABASE::ConstTileSetDatabasePtr pTileSetDB) : TileBrush(), m_pTileSetDB(pTileSetDB)
                {
                    m_BrushType = BRUSH_TILE_SET;
                }

                inline void setTileSetDB(DATABASE::ConstTileSetDatabasePtr pTileSetDB) { m_pTileSetDB = pTileSetDB; }

            private:
                DATABASE::ConstTileSetDatabasePtr m_pTileSetDB;
                UInt32Point3D m_centerPos;
            };

            class MapAutoTileBrush : public TileBrush
            {
            private:
                void _drawPen(MapViewer *pWidget, const UInt32Point3D &center);
                void _drawFill(MapViewer *pWidget, const UInt32Point3D &center);
                bool _checkFill(MapPrototypePtr map, MapTile centerTile, UInt32Point3D pos, BitsetVector &mapBitset, UInt32PointVector &openPoints);

                void _setAutoTile(MapPrototypePtr const &map, const UInt32Point3D &center, DATABASE::ConstAutoTilePrototypePtr proto);

                void _doAutoTileCheckForPosList(const MAP_STRUCTURE::MapPrototypePtr &map, const uint32 &uiLayer, const UInt32PointSet &posVector);

            public:
                MapAutoTileBrush(DATABASE::ConstAutoTileDatabasePtr pDB) : TileBrush(), m_pAutoTileDB(pDB)
                {
                    m_BrushType = BRUSH_AUTO_TILE;
                }

                inline void setAutoTileDB(DATABASE::ConstAutoTileDatabasePtr pAutoTileDB) { m_pAutoTileDB = pAutoTileDB; }

            private:
                DATABASE::ConstAutoTileDatabasePtr m_pAutoTileDB;
                UInt32PointSet m_BorderPosResult;
            };

            /*#####
            # MapObjectBrush
            #####*/
            class MapObjectBrush : public MapEditorObject
            {
            public:
                MapObjectBrush() : MapEditorObject() {}

                inline void setObjectType(DATABASE::MAP_OBJECT::ObjectType type) { m_ObjectType = type; }
                inline DATABASE::MAP_OBJECT::ObjectType getObjectType() const { return m_ObjectType; }

                inline void setObjectID(uint32 uiID) { m_uiObjectID = uiID; }
                inline uint32 getObjectID() const { return m_uiObjectID; }

                void updateObject() {}

                // return true if draw was successful
                bool drawObject(MapViewer *pWidget, Int32Point pos);

                static QPixmap getObjectPixmap(uint32 uiObjectID, DATABASE::MAP_OBJECT::ObjectType type, MAP_STRUCTURE::MapDirection direction,
                    DATABASE::ConstWorldObjectDatabasePtr pWorldObjectDB, DATABASE::ConstAnimationDatabasePtr pAnimationDB, DATABASE::ConstSpriteDatabasePtr pSpriteDB,
                    QRect &boundingRect);

            private:
                uint32 m_uiObjectID;
                DATABASE::MAP_OBJECT::ObjectType m_ObjectType;
            };
        }
        typedef std::shared_ptr<BRUSH::TileBrush> MapBrushPtr;
    }
}
#endif