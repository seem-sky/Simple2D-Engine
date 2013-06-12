#ifndef MAP_DATABASE_CHANGER_H
#define MAP_DATABASE_CHANGER_H

#include "DatabaseChanger.h"

namespace MAP
{
    typedef std::map<uint32, MapPrototypePtr> UInt32MapPrototypeMap;
    class MapDatabaseChanger : public DATABASE::DatabaseChanger<MAP::MapPrototype>
    {
    private:
        inline DATABASE::MapDatabasePtr _getTargetDB() { return boost::dynamic_pointer_cast<MapDatabase>(m_pTargetDB); }
        inline DATABASE::MapDatabasePtr _getNewDB() { return boost::dynamic_pointer_cast<MapDatabase>(m_pNewDB); }
    public:
        MapDatabaseChanger(DATABASE::MapDatabasePtr pMapDB);
        MapDatabaseChanger();

        bool removeMap(uint32 uiID);

        MapPrototypePtr getNewMap();

        bool loadMapFile(uint32 mapID, const QString &projectDir);
        void unloadMapFile(uint32 mapID);

        void storeChanges();
        void undoChanges(uint32 uiID);

    private:
        UInt32MapPrototypeMap m_removedMaps;
    };

    typedef boost::shared_ptr<MapDatabaseChanger> MapDatabaseChangerPtr;
}
#endif