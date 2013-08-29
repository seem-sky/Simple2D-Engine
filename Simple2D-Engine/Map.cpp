#include "Map.h"

using namespace MAP;

/*#####
# Map
#####*/
Map::Map(uint32 GUID, uint32 ID, MapLayer &layer, const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects, DATABASE::ConstDatabaseMgrPtr pDBMgr,
         const std::string &name, const std::string &script) :
    m_Layer(std::move(layer)), m_pDBMgr(pDBMgr), Entity(GUID, ID, name, script)
{
    _setupObjects(objects);
}

void Map::update(uint32 uiDiff)
{
    m_NewObjects.clear();
    //ENTITY::Object::update(uiDiff);

    // update objects
    for (uint32 i = 1; i <= m_Grid.getObjects().getSize(); ++i)
    {
        OBJECT::WorldObjectPtr pObject;
        if (m_Grid.getObjects().getItem(i, pObject))
            pObject->update(uiDiff);
    }
}

void Map::_setupObjects(const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects)
{
    for (uint16 i = 1; i <= objects.getSize(); ++i)
    {
        DATABASE::MAP_STRUCTURE::ConstMapObjectPtr obj;
        if (objects.getItem(i, obj))
        {
            OBJECT::WorldObjectPtr pWorldObject;
            switch (obj->m_Type)
            {
            case DATABASE::MAP_OBJECT::TYPE_WORLDOBJECT:
                {
                    DATABASE::ConstWorldObjectPrototypePtr pWObj;
                    if (!m_pDBMgr->getWorldObjectDatabase()->getItem(obj->m_ObjectID, pWObj))
                        continue;
                    pWorldObject = OBJECT::WorldObjectPtr(new OBJECT::WorldObject(m_pDBMgr, i, pWObj));
                    pWorldObject->setPosition(obj->m_Position);
                    pWorldObject->setDirection(obj->m_Direction);
                    break;
                }
            case DATABASE::MAP_OBJECT::TYPE_DYNAMIC_OBJECT:
                {
                    break;
                }
            }
            addWorldObject(pWorldObject);
        }
    }
}

void Map::addWorldObject(OBJECT::WorldObjectPtr pWorldObject)
{
    // add object to grid
    if (pWorldObject)
    {
        pWorldObject->setMap(this);
        m_Grid.addObject(pWorldObject);
        m_NewObjects.insert(pWorldObject);
    }
}

OBJECT::WorldObjectPtr Map::getWorldObject(uint32 GUID)
{
    OBJECT::WorldObjectPtr pObject;
    m_Grid.getObjects().getItem(GUID, pObject);
    return pObject;
}

/*#####
# non-member functions
#####*/
BitsetVector MAP::generatePassabilityMap(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer)
{
    BitsetVector passabilityMap;
    //UInt32Point size = mapLayer.getSize();
    //passabilityMap.resize(size.x, boost::dynamic_bitset<>(size.y));
    //for (uint32 x = 0; x < size.x; ++x)
    //{
    //    for (uint32 y = 0; y < size.y; ++y)
    //        passabilityMap.at(x)[y] = isLayerPassable(pDBMgr, mapLayer, UInt32Point(x, y), LAYER_BACKGROUND) && isLayerPassable(pDBMgr, mapLayer, UInt32Point(x, y), LAYER_FOREGROUND);
    //}
    return std::move(passabilityMap);
}

bool MAP::isLayerPassable(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer, UInt32Point pos, Layer layer)
{
    //DATABASE::ConstTileDatabasePtr pTileDB;
    //DATABASE::ConstAutoTileDatabasePtr pAutoTileDB;
    //if (!pDBMgr || !pTileDB || !pAutoTileDB)
    //    return false;

    //uint32 uiMaxLayer = mapLayer.getLayerSize(layer);
    //for (uint32 i = 0; i < uiMaxLayer; ++i)
    //{
        // ToDo: add check
        //MapTile tile = mapLayer.getMapTile(pos, layer);
        //if (tile.isAutoTile())
        //{
        //    if ()
        //}
        //else
        //{
        //    DATABASE::ConstTilePrototypePtr tileProto;
        //    if (!pTileDB->getItem(tile.m_uiTileID, tileProto))
        //        return false;
        //}
    //}
    return true;
}
