#include "MapAction.h"

using namespace MAP;
using namespace DATABASE;

/*#####
# MapAction
#####*/
MapAction::MapAction(const MapPrototypePtr &map) : m_pMap(map)
{
}

/*#####
# TileMapAction
#####*/
TileMapAction::TileMapAction(const Point3D<uint32> &pos, const MapTile &mapTile, const MapPrototypePtr &map) : MapAction(map), m_Pos(pos), m_MapTile(mapTile)
{
}

void TileMapAction::revertMapAction()
{
    if (!getMap())
        return;

    getMap()->setMapTile(m_Pos, m_MapTile);
    UInt32PointSet positions;
    if (m_MapTile.m_uiAutoTileSetID)
        getMap()->setTile(m_Pos, AutoTilePrototype::getAutoTileIndexForTileCheck(getMap()->getPositionsAroundWithID(m_MapTile.m_uiAutoTileSetID, m_Pos, positions)));
    for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, m_Pos.z);
        uint32 uiCurAutoTileID = getMap()->getAutoTile(pos);
        if (uiCurAutoTileID)
            getMap()->setTile(pos, AutoTilePrototype::getAutoTileIndexForTileCheck(getMap()->getPositionsAroundWithID(uiCurAutoTileID, pos,
                UInt32PointSet(), MapPrototype::FLAG_NOTHING)));
    }
}

/*#####
# TileMapAction
#####*/
MultiTileMapAction::MultiTileMapAction(const MapTile &mapTile, const MapPrototypePtr &map, const uint32 &uiLayer) : MapAction(map), m_MapTile(mapTile), m_uiLayer(uiLayer)
{
}

void MultiTileMapAction::revertMapAction()
{
    if (!getMap() || m_Positions.empty())
        return;

    // ugly solution, but i did not find a better way
    // first update all autotiles
    // after that, in a new loop, get all border tiles and update the tile if its an autotile set;
    // last update all border tiles
    UInt32PointSet positions;
    for (UInt32PointVector::const_iterator itr = m_Positions.begin(); itr != m_Positions.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, m_uiLayer);
        getMap()->setMapTile(pos, m_MapTile);
    }

    for (UInt32PointVector::const_iterator itr = m_Positions.begin(); itr != m_Positions.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, m_uiLayer);
        uint32 uiTileCheck = getMap()->getPositionsAroundWithID(m_MapTile.m_uiAutoTileSetID, pos, positions, MapPrototype::FLAG_OTHER);
        if (m_MapTile.m_uiAutoTileSetID)
            getMap()->setTile(pos, AutoTilePrototype::getAutoTileIndexForTileCheck(uiTileCheck));
    }

    for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, m_uiLayer);
        if (getMap()->getAutoTile(pos))
            getMap()->setTile(pos, AutoTilePrototype::getAutoTileIndexForTileCheck(getMap()->getPositionsAroundWithID(
                getMap()->getAutoTile(pos), pos, UInt32PointSet(), MapPrototype::FLAG_NOTHING)));
    }
}

void MultiTileMapAction::addPosition(const Point<uint32> &pos)
{
    m_Positions.push_back(pos);
}