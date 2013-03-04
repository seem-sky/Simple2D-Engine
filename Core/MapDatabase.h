#ifndef MAP_DATABASE_H
#define MAP_DATABASE_H

#include "Global.h"
#include "Point.h"
#include "Singleton.h"
#include "XML_Reader.h"
#include "XML_Writer.h"
#include <map>
#include <boost/smart_ptr.hpp>
#include <boost/multi_array.hpp>

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

    // map infos
    typedef boost::multi_array<uint32, 3> UInt32Multiarray3D;
    class MapPrototype
    {
        friend class MapWriter;
        friend class MapReader;
        friend class MapDatabase;
    public:
        MapPrototype(uint32 uiID = 0, const std::string &sFileName = "") : m_uiID(uiID), m_sFileName(sFileName), m_uiParentID(0), m_DataLoaded(false) {}

        inline bool hasMapDataStored() const { return m_DataLoaded; }

        inline std::string getFileName() const { return m_sFileName; }
        inline std::string getAnnounceName() const { return m_sAnnounceName; }
        inline std::string getScriptName() const { return m_sScriptName; }
        inline uint32 getID() const { return m_uiID; }
        inline Point3D<uint32> getSize() const { return m_Size; }

        inline uint32 getParentID() const { return m_uiParentID; }
        inline void setParentID(uint32 uiParentID) { m_uiParentID = uiParentID; }

        uint32 getTile(Point3D<uint32> at) const;
        void setTile(Point3D<uint32> at, uint32 uiID);

    private:
        bool m_DataLoaded;

        uint32 m_uiID;
        uint32 m_uiParentID;
        Point3D<uint32> m_Size;
        std::string m_sFileName;
        std::string m_sAnnounceName;
        std::string m_sScriptName;

        UInt32Multiarray3D m_MapData;
        MapObjectPtr2DVector m_Objects;
    };
    typedef boost::shared_ptr<MapPrototype> MapPrototypePtr;
    typedef std::map<uint32, MapPrototypePtr> MapPrototypePtrMap;

    class MapDatabase : public TSingleton<MapDatabase>
    {
    private:
        void _deleteRemovedMap(const MapPrototypePtr &map, const std::string &path);
        void _createNewMap(const MapPrototypePtr &map, const std::string &path);
        void _resizeMap(const MapPrototypePtr &map, Point3D<uint32> newSize) const;

    public:
        MapDatabase(void);

        bool removeMap(uint32 uiID);

        void loadMapDatabase(bool &result, const std::string &path = "");
        void saveMapDatabase(bool &result, const std::string &path = "");
        void saveMapInfo(const MapPrototypePtr &map, const std::string &path = "");
        void unloadMapDatabase();

        bool hasMapDataStored(uint32 uiIndex) const;
        bool loadMapFile(uint32 uiMapID, const std::string &sPath = "");
        void unloadMapFile(uint32 uiMapID);

        const MapPrototypePtrMap& getStoredMaps() const { return m_pMaps; }
        bool getMap(uint32 uiID, MapPrototypePtr &result) const;
        uint32 getFreeID() const;

        void setMapInfo(uint32 uiID, const std::string &sMapName, const std::string &sAnnounceName, const std::string &sScriptName, const Point3D<uint32> &size);

        static void getMapInfoFromXMLData(const XML::XML_ReadData &data, MapPrototypePtr &result);
        static bool getXMLDataFromMapInfo(const MapPrototypePtr &map, XML::XML_WriteData &result);
        static bool getFilePath(const MapPrototypePtr &map, std::string &result, const std::string path = "");
        static std::string getDefaultDBPath(const std::string &path = "");

    private:
        MapPrototypePtrMap m_pMaps;
        MapPrototypePtrMap m_pRemovedMaps;
    };
};
#endif