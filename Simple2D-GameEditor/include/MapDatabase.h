#ifndef MAP_DATABASE_H
#define MAP_DATABASE_H

#include "Global.h"
#include "Database.h"

namespace DATABASE
{
    class MapDatabaseXMLReader;
    namespace MAP_STRUCTURE
    {
        class MapDatabase : public DATABASE::Database<MapPrototype>
        {
        public:
            MapDatabase(void);

            bool removeMap(uint32 uiID);

            void clear();

            bool hasMapDataStored(uint32 uiIndex) const;
            bool loadMapFile(uint32 uiMapID, const QString &sPath = "");
            void unloadMapFile(uint32 uiMapID);

            static bool getFilePath(const MapPrototypePtr &map, QString &result, const QString path = "");

        private:
            MapPrototypePtrVector m_RemovedMaps;
        };
    }
}
#endif