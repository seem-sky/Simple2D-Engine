#ifndef MAP_BRUSH
#define MAP_BRUSH

#include "Global.h"
#include "MapView.h"

namespace MAP
{
    /*#####
    # TileBrushes
    #####*/
    class TileBrush
    {
    public:
        enum BrushType
        {
            BRUSH_PEN,
            BRUSH_RECT,
            BRUSH_FILL,
        };

    private:
        virtual void _drawPen(MapViewer *pWidget, const Point3D<uint32> &center) = 0;
        virtual void _drawFill(MapViewer *pWidget, const Point3D<uint32> &center) = 0;
        virtual bool _checkFill(MapPrototypePtr const &map, const MAP::MapTile &centerTile, Point3D<uint32> const &pos,
            std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints) = 0;

    protected:
        void calculateFillArea(MapViewer *pWidget, const Point3D<uint32> &center, BitsetVector &mapBitset = BitsetVector());
        void setupBitset(const MAP::MapPrototypePtr &map, BitsetVector &mapBitset);

    public:
        TileBrush();
        inline void setBrushType(BrushType type) { m_Type = type; }
        inline BrushType getBrushType() const { return m_Type; }
        inline void setTile(uint32 uiID) { m_uiTileID = uiID; }
        inline uint32 getTile() const { return m_uiTileID; }

        inline void setLayer(Layer layer) { m_Layer = layer; }
        inline Layer getLayer() const { return m_Layer; }

        virtual bool isAutoTileBrush() const = 0;

        void drawOnMap(MapViewer *pWidget, Point3D<uint32> center);

        void brushPress(MapViewer *pWidget, Point3D<uint32> center);
        void brushRelease(MapViewer *pWidget, Point3D<uint32> center);
        void brushMove(MapViewer *pWidget, Point3D<uint32> center);

    private:
        bool m_buttonHold;
        MapViewer *m_pLastEmitter;
        BrushType m_Type;
        uint32 m_uiTileID;
        Layer m_Layer;
    };
    typedef std::shared_ptr<TileBrush> MapBrushPtr;

    class MapTileBrush : public TileBrush
    {
    private:
        void _drawPen(MapViewer *pWidget, const Point3D<uint32> &center);
        void _drawFill(MapViewer *pWidget, const Point3D<uint32> &center);
        bool _checkFill(MapPrototypePtr const &map, const MAP::MapTile &centerTile, Point3D<uint32> const &pos,
            std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints);

    public:
        bool isAutoTileBrush() const { return false; }
    };

    class MapAutoTileBrush : public TileBrush
    {
    private:
        void _drawPen(MapViewer *pWidget, const Point3D<uint32> &center);
        void _drawFill(MapViewer *pWidget, const Point3D<uint32> &center);
        bool _checkFill(MapPrototypePtr const &map, const MAP::MapTile &centerTile, Point3D<uint32> const &pos,
            std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints);

        void _setAutoTile(MapPrototypePtr const &map, const Point3D<uint32> &center, DATABASE::ConstAutoTilePrototypePtr proto);

        void _doAutoTileCheckForPosList(const MAP::MapPrototypePtr &map, const uint32 &uiLayer, const UInt32PointSet &posVector);

    public:
        MapAutoTileBrush(DATABASE::ConstAutoTileDatabasePtr pDB) : TileBrush(), m_pAutoTileDB(pDB) {}
        inline void setAutoTileDB(DATABASE::ConstAutoTileDatabasePtr pAutoTileDB) { m_pAutoTileDB = pAutoTileDB; }

        bool isAutoTileBrush() const { return true; }

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

        inline void setObjectType(DATABASE::ObjectType type) { m_ObjectType = type; }
        inline DATABASE::ObjectType getObjectType() const { return m_ObjectType; }

        inline void setObjectID(uint32 uiID) { m_uiObjectID = uiID; }
        inline uint32 getObjectID() const { return m_uiObjectID; }

        void updateObject() {}

        // return true if draw was successful
        bool drawObject(MapViewer *pWidget, Point<int32> pos);

        static QPixmap getObjectPixmap(uint32 uiObjectID, DATABASE::ObjectType type, MAP::MapDirection direction,
            DATABASE::ConstWorldObjectDatabasePtr pWorldObjectDB, DATABASE::ConstAnimationDatabasePtr pAnimationDB, DATABASE::ConstSpriteDatabasePtr pSpriteDB,
            QRect &boundingRect);

    private:
        uint32 m_uiObjectID;
        DATABASE::ObjectType m_ObjectType;
    };
}
#endif