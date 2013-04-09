#ifndef MAP_DATABASE_H
#define MAP_DATABASE_H

#include "Global.h"
#include "Point.h"
#include <map>
#include <boost/multi_array.hpp>
#include "DatabasePrototypes.h"

namespace DATABASE
{
    class MapDatabaseXMLReader;
}

namespace MAP
{
    // map objects
    struct MapObject
    {
        uint32 m_ObjectID;
        uint32 m_GUID;
        Point<uint32> m_Position;
        uint32 m_uiDirection;
    };
    typedef boost::shared_ptr<MapObject> MapObjectPtr;
    typedef std::vector<MapObjectPtr> MapObjectPtrVector;
    typedef std::vector<MapObjectPtrVector> MapObjectPtr2DVector;

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
        inline void setFileName(const std::string &sFileName) { m_sFileName = sFileName; }
        void _resizeMap(const Point3D<uint32> &size);
        void _clear();

    public:
        MapPrototype(uint32 uiID = 0, const std::string &sFileName = "") : DATABASE::Prototype(uiID), m_sFileName(sFileName), m_uiParentID(0), m_DataLoaded(false)
        {}

        inline bool hasMapDataStored() const { return m_DataLoaded; }

        inline std::string getFileName() const { return m_sFileName; }
        inline void setScriptName(const std::string &sScriptName) { m_sScriptName = sScriptName; }
        inline std::string getScriptName() const { return m_sScriptName; }
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
        std::string m_sFileName;
        std::string m_sScriptName;

        TileDataMultiarray3D m_MapData;
        MapObjectPtr2DVector m_Objects;
    };
    typedef boost::shared_ptr<MapPrototype> MapPrototypePtr;
    typedef boost::shared_ptr<const MapPrototype> ConstMapPrototypePtr;
    typedef std::vector<MapPrototypePtr> MapPrototypePtrVector;
    typedef std::map<uint32, MapPrototypePtr> MapPrototypePtrMap;

    class MapDatabase : public DATABASE::Database<MapPrototype>
    {
    private:
        void _deleteRemovedMap(const MapPrototypePtr &map, const std::string &path);
        void _createNewMap(const MapPrototypePtr &map, const std::string &path);

    public:
        MapDatabase(void);

        bool removeMap(uint32 uiID);
        void deleteRemovedMaps(const std::string &path);

        void saveMapDatabase(bool &result, const std::string &path = "");
        void saveMapInfo(const MapPrototypePtr &map, const std::string &path = "");
        void clear();

        bool hasMapDataStored(uint32 uiIndex) const;
        bool loadMapFile(uint32 uiMapID, const std::string &sPath = "");
        void unloadMapFile(uint32 uiMapID);

        MapPrototypePtr getNewMap();

        static bool getFilePath(const MapPrototypePtr &map, std::string &result, const std::string path = "");
        static std::string getDefaultDBPath(const std::string &path = "");

    private:
        MapPrototypePtrVector m_RemovedMaps;
    };
};
#endif