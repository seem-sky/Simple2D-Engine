#ifndef MAP_DATABASE_H
#define MAP_DATABASE_H

#include "Point.h"
#include <map>
#include "Global.h"
#include "DatabasePrototypes.h"
#include <boost/multi_array.hpp>

namespace DATABASE
{
    class MapDatabaseXMLReader;
}

namespace MAP
{
    enum MapDirection
    {
        DIRECTION_UP,
        DIRECTION_RIGHT,
        DIRECTION_DOWN,
        DIRECTION_LEFT
    };

    enum MapObjectLayer
    {
        LAYER_LOWER,
        LAYER_MIDDLE,
        LAYER_UPPER
    };

    // map objects
    struct MapObject
    {
        MapObject() : m_Type(DATABASE::TYPE_WORLDOBJECT), m_ObjectID(0), m_GUID(0), m_Direction(DIRECTION_DOWN), m_Layer(LAYER_MIDDLE) {}
        DATABASE::ObjectType m_Type;
        uint32 m_ObjectID;
        uint32 m_GUID;
        Point3D<uint32> m_Position;
        MapDirection m_Direction;
        MapObjectLayer m_Layer;
    };
    typedef boost::shared_ptr<MapObject> MapObjectPtr;
    typedef std::vector<MapObjectPtr> MapObjectPtrVector;
    typedef boost::multi_array<MapObjectPtrVector, 3> MapObjectPtrVectorMultiarray3D;

    struct MapTile
    {
        MapTile(uint32 uiTileID = 0, uint32 uiAutoTileSetID = 0) : m_uiAutoTileSetID(uiAutoTileSetID), m_uiTileID(uiTileID) {}
        uint32 m_uiTileID;
        uint32 m_uiAutoTileSetID;
    };
    typedef std::vector<MapTile> MapTileVector;

    enum Layer
    {
        LAYER_BACKGROUND,
        LAYER_FOREGROUND
    };

    // map infos
    typedef boost::multi_array<uint32, 3> UInt32Multiarray3D;
    typedef boost::multi_array<MapTile, 3> TileDataMultiarray3D;
    class MapPrototype : public DATABASE::Prototype
    {
        friend class MapReader;
        friend class MapWriter;
        friend class MapDatabase;
        friend class DATABASE::MapDatabaseXMLReader;
    private:
        inline void setFileName(const QString &sFileName) { m_sFileName = sFileName; }
        void _resizeMap(Point<uint32> size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize);
        void _clearTiles();

    public:
        MapPrototype(uint32 uiID = 0, const QString &sFileName = "") : DATABASE::Prototype(uiID), m_sFileName(sFileName), m_uiParentID(0), m_DataLoaded(false),
            m_uiBackgroundLayer(0), m_uiForegroundLayer(0)
        {}

        inline bool hasMapDataStored() const { return m_DataLoaded; }

        inline QString getFileName() const { return m_sFileName; }
        inline void setScriptName(const QString &sScriptName) { m_sScriptName = sScriptName; }
        inline QString getScriptName() const { return m_sScriptName; }
        void setSize(Point<uint32> size, uint32 uiForegroundLayerSize, uint32 uiBackgroundLayerSize);
        inline Point<uint32> getSize() const { return m_Size; }
        uint32 getLayerSize(Layer layer) const;

        inline uint32 getParentID() const { return m_uiParentID; }
        inline void setParentID(uint32 uiParentID) { m_uiParentID = uiParentID; }

        uint32 getTile(Point3D<uint32> at, Layer layer = LAYER_BACKGROUND) const;
        void setTile(Point3D<uint32> at, uint32 uiID, Layer layer = LAYER_BACKGROUND);

        uint32 getAutoTile(Point3D<uint32> at, Layer layer = LAYER_BACKGROUND) const;
        void setAutoTile(Point3D<uint32> at, uint32 uiID, Layer layer = LAYER_BACKGROUND);

        void setMapTile(Point3D<uint32> at, MapTile mapTile, Layer layer = LAYER_BACKGROUND);
        MapTile getMapTile(Point3D<uint32> at, Layer layer = LAYER_BACKGROUND) const;

        void addMapObject(MapObjectPtr pObject);
        MapObjectPtr addMapObject(DATABASE::ObjectType type, uint32 uiID, Point3D<uint32> pos);
        void moveMapObject(uint32 uiGUID, const Point3D<uint32> &newPos);

        inline bool isInMap(Point<uint32> at) const { return at.x < m_Size.x && at.y < m_Size.y; }

        enum RESULT_FLAG
        {
            FLAG_NOTHING    = 0x0,
            FLAG_SAME       = 0x1,
            FLAG_OTHER      = 0x2,
            FLAG_ALL        = FLAG_SAME | FLAG_OTHER
        };
        uint32 getPositionsAroundWithID(const uint32 &uiID, const Point3D<uint32> &pos, UInt32PointSet &result, uint32 resultFlag = FLAG_ALL);

    private:
        bool m_DataLoaded;

        uint32 m_uiParentID;
        Point<uint32> m_Size;
        uint32 m_uiBackgroundLayer;
        uint32 m_uiForegroundLayer;
        QString m_sFileName;
        QString m_sScriptName;

        MapObjectPtrVector m_Objects;
        TileDataMultiarray3D m_BackgroundTiles;
        TileDataMultiarray3D m_ForegroundTiles;
    };
    typedef boost::shared_ptr<MapPrototype> MapPrototypePtr;
    typedef boost::shared_ptr<const MapPrototype> ConstMapPrototypePtr;
    typedef std::vector<MapPrototypePtr> MapPrototypePtrVector;
    typedef std::map<uint32, MapPrototypePtr> MapPrototypePtrMap;

    class MapDatabase : public DATABASE::Database<MapPrototype>
    {
    private:
        //void _deleteRemovedMap(const MapPrototypePtr &map, const QString &path);
        //void _createNewMap(const MapPrototypePtr &map, const QString &path);

    public:
        MapDatabase(void);

        bool removeMap(uint32 uiID);
        //void deleteRemovedMaps(const QString &path);

        void clear();

        bool hasMapDataStored(uint32 uiIndex) const;
        bool loadMapFile(uint32 uiMapID, const QString &sPath = "");
        void unloadMapFile(uint32 uiMapID);

        static bool getFilePath(const MapPrototypePtr &map, QString &result, const QString path = "");

    private:
        MapPrototypePtrVector m_RemovedMaps;
    };
};
#endif