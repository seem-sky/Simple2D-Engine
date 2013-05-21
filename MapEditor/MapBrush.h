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
        virtual void _drawPen(MapViewWidget *pWidget, const Point3D<uint32> &center) = 0;
        virtual void _drawFill(MapViewWidget *pWidget, const Point3D<uint32> &center) = 0;
        virtual bool _checkFill(MapPrototypePtr const &map, const MAP::MapTile &centerTile, Point3D<uint32> const &pos,
            std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints) = 0;

    protected:
        void calculateFillArea(MapViewWidget *pWidget, const Point3D<uint32> &center, BitsetVector &mapBitset = BitsetVector());
        void setupBitset(const MAP::MapPrototypePtr &map, BitsetVector &mapBitset);

    public:
        TileBrush();
        inline void setBrushType(BrushType type) { m_Type = type; }
        inline BrushType getBrushType() const { return m_Type; }
        inline void setTile(uint32 uiID) { m_uiTileID = uiID; }
        inline uint32 getTile() const { return m_uiTileID; }

        void drawOnMap(MapViewWidget *pWidget, Point3D<uint32> center);

        void brushPress(MapViewWidget *pWidget, Point3D<uint32> center);
        void brushRelease(MapViewWidget *pWidget, Point3D<uint32> center);
        void brushMove(MapViewWidget *pWidget, Point3D<uint32> center);
        
    private:
        bool m_buttonHold;
        MapViewWidget *m_pLastEmitter;
        BrushType m_Type;
        uint32 m_uiTileID;
    };
    typedef boost::shared_ptr<TileBrush> MapBrushPtr;

    class MapTileBrush : public TileBrush
    {
    private:
        void _drawPen(MapViewWidget *pWidget, const Point3D<uint32> &center);
        void _drawFill(MapViewWidget *pWidget, const Point3D<uint32> &center);
        bool _checkFill(MapPrototypePtr const &map, const MAP::MapTile &centerTile, Point3D<uint32> const &pos,
            std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints);
    };

    class MapAutoTileBrush : public TileBrush
    {
    private:
        void _drawPen(MapViewWidget *pWidget, const Point3D<uint32> &center);
        void _drawFill(MapViewWidget *pWidget, const Point3D<uint32> &center);
        bool _checkFill(MapPrototypePtr const &map, const MAP::MapTile &centerTile, Point3D<uint32> const &pos,
            std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints);

        void _setAutoTile(MapPrototypePtr const &map, const Point3D<uint32> &center, DATABASE::ConstAutoTilePrototypePtr proto);

        void _doAutoTileCheckForPosList(const MAP::MapPrototypePtr &map, const uint32 &uiLayer, const UInt32PointSet &posVector);

    public:
        MapAutoTileBrush(DATABASE::ConstAutoTileDatabasePtr pDB) : TileBrush(), m_pAutoTileDB(pDB) {}
        inline void setAutoTileDB(DATABASE::ConstAutoTileDatabasePtr pAutoTileDB) { m_pAutoTileDB = pAutoTileDB; }

    private:
        DATABASE::ConstAutoTileDatabasePtr m_pAutoTileDB;
        UInt32PointSet m_BorderPosResult;
    };

    /*#####
    # MapObjectBrush
    #####*/
    class MapObjectBrush
    {
    public:
        inline void setObjectType(DATABASE::ObjectType type) { m_ObjectType = type; }
        inline DATABASE::ObjectType getObjectType() const { return m_ObjectType; }

        inline void setObjectID(uint32 uiID) { m_uiObjectID = uiID; }
        inline uint32 getObjectID() const { return m_uiObjectID; }

        // return true if draw was successful
        bool drawObject(MapViewWidget *pWidget, Point3D<uint32> pos);

        void setAdditionalDBs(DATABASE::ConstWorldObjectDatabasePtr pWorldObjectDB, DATABASE::ConstAnimationDatabasePtr pAnimationDB,
            DATABASE::ConstSpriteDatabasePtr pSpriteDB);

    private:
        uint32 m_uiObjectID;
        DATABASE::ObjectType m_ObjectType;

        DATABASE::ConstWorldObjectDatabasePtr m_pWorldObjectDB;
        DATABASE::ConstAnimationDatabasePtr m_pAnimationDB;
        DATABASE::ConstSpriteDatabasePtr m_pSpriteDB;
    };
}
#endif