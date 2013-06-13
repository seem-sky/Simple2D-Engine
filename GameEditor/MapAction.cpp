#include "MapAction.h"

using namespace MAP;
using namespace DATABASE;

/*#####
# MapAction
#####*/
MapAction::MapAction(MapPrototypePtr map) : m_pMap(map)
{
}

/*#####
# TileMapAction
#####*/
TileMapAction::TileMapAction(Point3D<uint32> pos, MapTile mapTile, Layer layer, MapPrototypePtr map) : MapAction(map), m_Pos(pos), m_MapTile(mapTile), m_Layer(layer)
{}

void TileMapAction::revertMapAction()
{
    if (!getMap())
        return;

    getMap()->setMapTile(m_Pos, m_MapTile, m_Layer);
    UInt32PointSet positions;
    if (m_MapTile.m_uiAutoTileSetID)
        getMap()->setTile(m_Pos, AutoTilePrototype::getAutoTileIndexForTileCheck(getMap()->getPositionsAroundWithID(m_MapTile.m_uiAutoTileSetID, m_Pos, positions, m_Layer)), m_Layer);
    for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, m_Pos.z);
        uint32 uiCurAutoTileID = getMap()->getAutoTile(pos, m_Layer);
        if (uiCurAutoTileID)
            getMap()->setTile(pos, AutoTilePrototype::getAutoTileIndexForTileCheck(getMap()->getPositionsAroundWithID(uiCurAutoTileID, pos,
            UInt32PointSet(), m_Layer, MapPrototype::FLAG_NOTHING)), m_Layer);
    }
}

/*#####
# TileMapAction
#####*/
MultiTileMapAction::MultiTileMapAction(MapTile mapTile, MapPrototypePtr map, uint32 uiLayer, Layer layer) : MapAction(map), m_MapTile(mapTile), m_uiLayer(uiLayer), m_Layer(layer)
{}

void MultiTileMapAction::revertMapAction()
{
    if (!getMap() || m_Positions.empty())
        return;

    // ugly solution, but i did not find a better way
    // first update all autotiles
    // after that, in a new loop, get all border tiles and update the tile
    // last update all border tiles
    UInt32PointSet positions;
    for (UInt32PointVector::const_iterator itr = m_Positions.begin(); itr != m_Positions.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, m_uiLayer);
        getMap()->setMapTile(pos, m_MapTile, m_Layer);
    }

    for (UInt32PointVector::const_iterator itr = m_Positions.begin(); itr != m_Positions.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, m_uiLayer);
        uint32 uiTileCheck = getMap()->getPositionsAroundWithID(m_MapTile.m_uiAutoTileSetID, pos, positions, m_Layer, MapPrototype::FLAG_OTHER);
        if (m_MapTile.m_uiAutoTileSetID)
            getMap()->setTile(pos, AutoTilePrototype::getAutoTileIndexForTileCheck(uiTileCheck), m_Layer);
    }

    for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, m_uiLayer);
        if (getMap()->getAutoTile(pos, m_Layer))
            getMap()->setTile(pos, AutoTilePrototype::getAutoTileIndexForTileCheck(getMap()->getPositionsAroundWithID(
                getMap()->getAutoTile(pos, m_Layer), pos, UInt32PointSet(), m_Layer, MapPrototype::FLAG_NOTHING)), m_Layer);
    }
}

void MultiTileMapAction::addPosition(const Point<uint32> &pos)
{
    m_Positions.push_back(pos);
}