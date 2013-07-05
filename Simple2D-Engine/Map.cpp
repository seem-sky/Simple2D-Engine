#include "Map.h"

using namespace MAP;

/*#####
# Map
#####*/
Map::Map(uint32 uiID, MapLayer &layer) : m_Layer(std::move(layer)), m_uiID(uiID)
{
    m_TransformationHolder.addTransformation(TRANSFORMATION::MovePtr(new TRANSFORMATION::Move(5*IN_MILLISECOND, m_Position, Int32Point(-64, 0))));
    m_TransformationHolder.addTransformation(TRANSFORMATION::MovePtr(new TRANSFORMATION::Move(5*IN_MILLISECOND, m_Position, Int32Point(0, -64))));
    m_TransformationHolder.addTransformation(TRANSFORMATION::MovePtr(new TRANSFORMATION::Move(5*IN_MILLISECOND, m_Position, Int32Point(64, 0))));
    m_TransformationHolder.addTransformation(TRANSFORMATION::MovePtr(new TRANSFORMATION::Move(5*IN_MILLISECOND, m_Position, Int32Point(0, 64))));
}

void Map::update(uint32 uiDiff)
{
    m_TransformationHolder.updateTransformations(uiDiff);
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
