#include "Map.h"

using namespace MAP;

/*#####
# Map
#####*/
Map::Map(uint32 GUID, uint32 uiID, const std::string &name, MapLayer &layer, const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects,
         DATABASE::ConstDatabaseMgrPtr pDBMgr) :
    m_Layer(std::move(layer)), m_pDBMgr(pDBMgr), Object(GUID, uiID, name)
{
    move(Int32Point(0, 400), 5*IN_MILLISECOND);
    _setupObjects(objects);
}

void Map::update(uint32 uiDiff)
{
    m_NewObjects.clear();
    ENTITY::Object::update(uiDiff);

    // update objects
    for (uint32 i = 1; i < m_Grid.getObjects().getSize(); ++i)
    {
        OBJECT::WorldObjectPtr pObject;
        if (m_Grid.getObjects().getItem(i, pObject))
            pObject->update(uiDiff);
    }
}

void Map::_setupObjects(const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects)
{
    for (uint32 j = 0; j < 100; ++j)
    {
        Int32Point point(j*10, j*10);
        for (uint32 i = 1; i < objects.getSize(); ++i)
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
                        pWorldObject = OBJECT::WorldObjectPtr(new OBJECT::WorldObject(i+100*j, pWObj));
                        pWorldObject->setPosition(obj->m_Position+point);
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
}

void Map::addWorldObject(OBJECT::WorldObjectPtr pWorldObject)
{
    // add object to grid
    if (pWorldObject)
    {
        m_Grid.addObject(pWorldObject);
        m_NewObjects.insert(pWorldObject);
        pWorldObject->setDBMgr(m_pDBMgr);
    }
}

/*#####
# non-member functions
#####*/
BitsetVector MAP::generatePassabilityMap(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer)
{
    BitsetVector passabilityMap;
    UInt32Point size = mapLayer.getSize();
    passabilityMap.resize(size.x, boost::dynamic_bitset<>(size.y));
    for (uint32 x = 0; x < size.x; ++x)
    {
        for (uint32 y = 0; y < size.y; ++y)
            passabilityMap.at(x)[y] = isLayerPassable(pDBMgr, mapLayer, UInt32Point(x, y), LAYER_BACKGROUND) && isLayerPassable(pDBMgr, mapLayer, UInt32Point(x, y), LAYER_FOREGROUND);
    }
    return std::move(passabilityMap);
}

bool MAP::isLayerPassable(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer, UInt32Point pos, Layer layer)
{
    DATABASE::ConstTileDatabasePtr pTileDB;
    DATABASE::ConstAutoTileDatabasePtr pAutoTileDB;
    if (!pDBMgr || !pTileDB || !pAutoTileDB)
        return false;

    uint32 uiMaxLayer = mapLayer.getLayerSize(layer);
    for (uint32 i = 0; i < uiMaxLayer; ++i)
    {
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
    }
    return true;
}
