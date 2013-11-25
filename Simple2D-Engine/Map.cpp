#include "Map.h"

using namespace MAP;

/*#####
# Map
#####*/
Map::Map(uint32 GUID, uint32 ID, MapLayer &layer, const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects, const DATABASE::DatabaseMgr &DBMgr,
         const std::string &name, const std::string &script) : m_Layer(std::move(layer)), m_DBMgr(DBMgr), Entity(GUID, ID, name, script)
{
    _setupObjects(objects);
}

void Map::update(uint32 uiDiff)
{
    m_NewObjects.clear();

    // update objects
    for (uint32 i = 1; i <= m_Grid.getObjects().getSize(); ++i)
    {
        if (auto pObject = m_Grid.getObjects().getItem(i))
            pObject->update(uiDiff);
    }
}

void Map::_setupObjects(const DATABASE::MAP_STRUCTURE::MapObjectContainer &objects)
{
    for (uint16 i = 1; i <= objects.getSize(); ++i)
    {
        if (auto pObj = objects.getItem(i))
        {
            OBJECT::WorldObject *pWorldObject(nullptr);
            switch (pObj->m_Type)
            {
            case DATABASE::MAP_OBJECT::TYPE_WORLDOBJECT:
                {
                    pWorldObject = new OBJECT::WorldObject(m_DBMgr, i, pObj->m_ObjectID);
                    pWorldObject->setPosition(pObj->m_Position);
                    pWorldObject->setDirection(pObj->m_Direction);
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

void Map::addWorldObject(OBJECT::WorldObject *pWorldObject)
{
    // add object to grid
    if (pWorldObject)
    {
        pWorldObject->setMap(this);
        m_Grid.addObject(pWorldObject);
        m_NewObjects.insert(pWorldObject);
    }
}

/*#####
# non-member functions
#####*/
// ToDo:
//BitsetVector MAP::generatePassabilityMap(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer)
//{
//    BitsetVector passabilityMap;
//    //UInt32Point size = mapLayer.getSize();
//    //passabilityMap.resize(size.x, boost::dynamic_bitset<>(size.y));
//    //for (uint32 x = 0; x < size.x; ++x)
//    //{
//    //    for (uint32 y = 0; y < size.y; ++y)
//    //        passabilityMap.at(x)[y] = isLayerPassable(pDBMgr, mapLayer, UInt32Point(x, y), LAYER_BACKGROUND) && isLayerPassable(pDBMgr, mapLayer, UInt32Point(x, y), LAYER_FOREGROUND);
//    //}
//    return std::move(passabilityMap);
//}
//
//bool MAP::isLayerPassable(DATABASE::ConstDatabaseMgrPtr pDBMgr, const MapLayer &mapLayer, UInt32Point pos, Layer layer)
//{
//    //DATABASE::ConstTileDatabasePtr pTileDB;
//    //DATABASE::ConstAutoTileDatabasePtr pAutoTileDB;
//    //if (!pDBMgr || !pTileDB || !pAutoTileDB)
//    //    return false;
//
//    //uint32 uiMaxLayer = mapLayer.getLayerSize(layer);
//    //for (uint32 i = 0; i < uiMaxLayer; ++i)
//    //{
//        // ToDo: add check
//        //MapTile tile = mapLayer.getMapTile(pos, layer);
//        //if (tile.isAutoTile())
//        //{
//        //    if ()
//        //}
//        //else
//        //{
//        //    DATABASE::ConstTilePrototypePtr tileProto;
//        //    if (!pTileDB->getItem(tile.m_uiTileID, tileProto))
//        //        return false;
//        //}
//    //}
//    return true;
//}
