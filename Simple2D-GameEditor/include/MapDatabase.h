#ifndef MAP_DATABASE_H
#define MAP_DATABASE_H

#include <Global.h>
#include "Database.h"

namespace DATABASE
{
    typedef std::unique_ptr<PROTOTYPE::MAP_STRUCTURE::MapPrototype> MapPrototypePtr;
    typedef std::vector<std::unique_ptr<PROTOTYPE::MAP_STRUCTURE::MapPrototype>> MapPrototypePtrVector;
    class MapDatabase : public Database<PROTOTYPE::MAP_STRUCTURE::MapPrototype>
    {
    public:
        bool removeMap(uint32 uiID);

        void clear();

        MapPrototypePtr getNewPrototype();

        /*void setPrototype(MapPrototype* pItem);
        const MapPrototype* getPrototype(uint32 ID) const;*/
        //MapPrototype* getPrototype(uint32 ID);

        //MapPrototype* getNewPrototype(uint32 ID = 0) const;

    private:
        MapPrototypePtrVector m_RemovedMaps;
    };
}
#endif