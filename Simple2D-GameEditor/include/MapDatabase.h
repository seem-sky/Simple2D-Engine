#ifndef MAP_DATABASE_H
#define MAP_DATABASE_H

#include <Global.h>
#include "Database.h"

namespace DATABASE
{
    namespace MAP_STRUCTURE
    {
        const char MAP_FILE_ENDING[] = ".map";
        const char MAP_FOLDER[] = "/Maps/";

        typedef std::unique_ptr<MapPrototype> MapPrototypePtr;
        typedef std::vector<std::unique_ptr<MapPrototype>> MapPrototypePtrVector;
        class MapDatabase : public Database<MapPrototype>
        {
        public:
            bool removeMap(uint32 uiID);

            void clear();

            MapPrototypePtr getNewPrototype();

            /*void setPrototype(MapPrototype *pItem);
            const MapPrototype* getPrototype(uint32 ID) const;*/
            //MapPrototype* getPrototype(uint32 ID);

            //MapPrototype* getNewPrototype(uint32 ID = 0) const;

        private:
            MapPrototypePtrVector m_RemovedMaps;
        };
    }
}
#endif