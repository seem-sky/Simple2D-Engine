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
    enum MAP_DIRECTION
    {
        DIRECTION_UP,
        DIRECTION_RIGHT,
        DIRECTION_DOWN,
        DIRECTION_LEFT
    };

    // map objects
    struct MapObject
    {
        MapObject() : m_Type(DATABASE::TYPE_WORLDOBJECT), m_ObjectID(0), m_GUID(0), m_Direction(DIRECTION_DOWN) {}
        DATABASE::ObjectType m_Type;
        uint32 m_ObjectID;
        uint32 m_GUID;
        Point3D<uint32> m_Position;
        MAP_DIRECTION m_Direction;
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
        void _resizeMap(const Point3D<uint32> &size);
        void _clear();

    public:
        MapPrototype(uint32 uiID = 0, const QString &sFileName = "") : DATABASE::Prototype(uiID), m_sFileName(sFileName), m_uiParentID(0), m_DataLoaded(false)
        {}

        inline bool hasMapDataStored() const { return m_DataLoaded; }

        inline QString getFileName() const { return m_sFileName; }
        inline void setScriptName(const QString &sScriptName) { m_sScriptName = sScriptName; }
        inline QString getScriptName() const { return m_sScriptName; }
        void setSize(const Point3D<uint32> &size);
        inline Point3D<uint32> getSize() const { return m_Size; }

        inline uint32 getParentID() const { return m_uiParentID; }
        inline void setParentID(uint32 uiParentID) { m_uiParentID = uiParentID; }

        uint32 getTile(const Point3D<uint32> &at) const;
        void setTile(const Point3D<uint32> &at, uint32 uiID);

        uint32 getAutoTile(const Point3D<uint32> &at) const;
        void setAutoTile(const Point3D<uint32> &at, uint32 uiID);

        void setMapTile(const Point3D<uint32> &at, const MapTile &mapTile);
        MapTile getMapTile(const Point3D<uint32> &at) const;

        MapObjectPtr addMapObject(DATABASE::ObjectType type, uint32 uiID, Point3D<uint32> pos);
        void moveMapObject(uint32 uiGUID, const Point3D<uint32> &newPos);

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
        Point3D<uint32> m_Size;
        QString m_sFileName;
        QString m_sScriptName;

        MapObjectPtrVector m_Objects;
        TileDataMultiarray3D m_MapData;
    };
    typedef boost::shared_ptr<MapPrototype> MapPrototypePtr;
    typedef boost::shared_ptr<const MapPrototype> ConstMapPrototypePtr;
    typedef std::vector<MapPrototypePtr> MapPrototypePtrVector;
    typedef std::map<uint32, MapPrototypePtr> MapPrototypePtrMap;

    class MapDatabase : public DATABASE::Database<MapPrototype>
    {
    private:
        void _deleteRemovedMap(const MapPrototypePtr &map, const QString &path);
        void _createNewMap(const MapPrototypePtr &map, const QString &path);

    public:
        MapDatabase(void);

        bool removeMap(uint32 uiID);
        void deleteRemovedMaps(const QString &path);

        void saveMapDatabase(bool &result, const QString &path = "");
        void saveMapInfo(const MapPrototypePtr &map, const QString &path = "");
        void clear();

        bool hasMapDataStored(uint32 uiIndex) const;
        bool loadMapFile(uint32 uiMapID, const QString &sPath = "");
        void unloadMapFile(uint32 uiMapID);

        MapPrototypePtr getNewMap();

        static bool getFilePath(const MapPrototypePtr &map, QString &result, const QString path = "");
        static QString getDefaultDBPath(const QString &path = "");

    private:
        MapPrototypePtrVector m_RemovedMaps;
    };
};
#endif