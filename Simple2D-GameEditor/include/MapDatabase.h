#ifndef MAP_DATABASE_H
#define MAP_DATABASE_H

#include <Global.h>
#include "Database.h"

namespace DATABASE
{
    class MapDatabaseXMLReader;
    namespace MAP_STRUCTURE
    {
        const char MAP_FILE_ENDING[] = ".map";
        const char MAP_FOLDER[] = "/Maps/";

        class MapDatabase : public DATABASE::Database<MapPrototype>
        {
        public:
            MapDatabase(void);

            bool removeMap(uint32 uiID);

            void clear();

            bool hasMapDataStored(uint32 uiIndex) const;
            bool loadMapFile(uint32 uiMapID, const QString &sPath = "");
            void unloadMapFile(uint32 uiMapID);

        private:
            MapPrototypePtrVector m_RemovedMaps;
        };

        void saveMapFile(ConstMapPrototypePtr pMap, const QString &path);
    }
}
#endif