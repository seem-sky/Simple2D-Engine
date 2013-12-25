#include "MapDatabase.h"
#include "MapBinaryIO.h"

using namespace DATABASE;
using namespace DATABASE::MAP_STRUCTURE;

void MapDatabase::clear()
{
    Database::clear();
    m_RemovedMaps.clear();
}

bool MapDatabase::removeMap(uint32 uiID)
{
    if (!getOriginalPrototype(uiID))
        return false;
    // ToDo: setPrototype(uiID, nullptr);
    return true;
}

MapPrototypePtr MapDatabase::getNewPrototype()
{
    // search for invalid prototypes
    for (auto& pPrototype : getPrototypes())
    {
        if (!pPrototype->isValid())
            return MapPrototypePtr(new MapPrototype(*pPrototype));
    }

    return MapPrototypePtr(new MapPrototype(getSize()+1));
}

//void MapDatabase::setPrototype(MapPrototype* pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const MapPrototype* MapDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const MapPrototype*>(Database::getPrototype(ID));
//}
//
//MapPrototype* MapDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<MapPrototype*>(Database::getPrototype(ID));
//}
//
//MapPrototype* MapDatabase::getNewPrototype(uint32 ID) const
//{
//    return new MapPrototype(ID);
//}
