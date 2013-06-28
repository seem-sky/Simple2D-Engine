#include "MapAction.h"

using namespace MAP;
using namespace DATABASE;
using namespace AUTO_TILE;

/*#####
# MapAction
#####*/
MapAction::MapAction(MapPrototypePtr map) : m_pMap(map)
{
}

/*#####
# TileMapAction
#####*/
TileMapAction::TileMapAction(UInt32Point3D pos, MapTile mapTile, Layer layer, MapPrototypePtr map) : MapAction(map), m_Pos(pos), m_MapTile(mapTile), m_Layer(layer)
{}

void TileMapAction::revertMapAction()
{
    if (!getMap())
        return;

    getMap()->setMapTile(m_Pos, m_MapTile, m_Layer);
    UInt32PointSet positions;
    if (m_MapTile.m_uiAutoTileSetID)
        getMap()->setTile(m_Pos, getAutoTileIndexForTileCheck(getMap()->checkAutoTiles(m_MapTile.m_uiAutoTileSetID, m_Pos, positions, m_Layer)), m_Layer);
    for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
    {
        UInt32Point3D pos(*itr, m_Pos.z);
        uint32 uiCurAutoTileID = getMap()->getAutoTile(pos, m_Layer);
        if (uiCurAutoTileID)
            getMap()->setTile(pos, getAutoTileIndexForTileCheck(getMap()->checkAutoTiles(uiCurAutoTileID, pos,
            UInt32PointSet(), m_Layer, MapPrototype::FLAG_NOTHING)), m_Layer);
    }
}

/*#####
# MultiPositionTileMapAction
#####*/
MultiPositionTileMapAction::MultiPositionTileMapAction(MapTile mapTile, MapPrototypePtr map, uint32 uiLayer, Layer layer) :
    MapAction(map), m_MapTile(mapTile), m_uiLayer(uiLayer), m_Layer(layer)
{}

void MultiPositionTileMapAction::revertMapAction()
{
    if (!getMap() || m_Positions.empty())
        return;

    // ugly solution, but i did not find a better way
    // first update all autotiles
    // after that, in a new loop, get all border tiles and update the tile
    // last update all border tiles
    UInt32PointSet positions;
    std::for_each(m_Positions.cbegin(), m_Positions.cend(), [this] (UInt32Point pos)
    {
        UInt32Point3D curPos(pos, m_uiLayer);
        getMap()->setMapTile(curPos, m_MapTile, m_Layer);
    });

    std::for_each(m_Positions.cbegin(), m_Positions.cend(), [this, &positions] (UInt32Point pos)
    {
        UInt32Point3D curPos(pos, m_uiLayer);
        uint32 uiTileCheck = getMap()->checkAutoTiles(m_MapTile.m_uiAutoTileSetID, curPos, positions, m_Layer, MapPrototype::FLAG_OTHER);
        if (m_MapTile.m_uiAutoTileSetID)
            getMap()->setTile(curPos, getAutoTileIndexForTileCheck(uiTileCheck), m_Layer);
    });

    std::for_each(positions.cbegin(), positions.cend(), [this] (UInt32Point pos)
    {
        UInt32Point3D curPos(pos, m_uiLayer);
        if (getMap()->getAutoTile(curPos, m_Layer))
            getMap()->setTile(curPos, getAutoTileIndexForTileCheck(getMap()->checkAutoTiles(
            getMap()->getAutoTile(curPos, m_Layer), curPos, UInt32PointSet(), m_Layer, MapPrototype::FLAG_NOTHING)), m_Layer);
    });
}

void MultiPositionTileMapAction::addPosition(UInt32Point pos)
{
    m_Positions.push_back(pos);
}

/*#####
# MultiPositionMultiTileMapAction
#####*/
MultiPositionMultiTileMapAction::MultiPositionMultiTileMapAction(MapPrototypePtr map, uint32 uiLayer, Layer layer) :
    MapAction(map), m_uiLayer(uiLayer), m_Layer(layer)
{}

void MultiPositionMultiTileMapAction::addPosition(UInt32Point pos, MapTile tile)
{
    m_Positions.push_back(std::make_pair(pos, tile));
}

void MultiPositionMultiTileMapAction::revertMapAction()
{
    if (!getMap())
        return;

    std::for_each(m_Positions.cbegin(), m_Positions.cend(), [this](UInt32PointMapTilePair pair) { _revertAction(pair); } );
}

void MultiPositionMultiTileMapAction::_revertAction(UInt32PointMapTilePair pair)
{
    UInt32Point3D pos(pair.first, m_uiLayer);
    getMap()->setMapTile(pos, pair.second, m_Layer);
    if (pair.second.m_uiAutoTileSetID)
    {
        UInt32PointSet positions;
        getMap()->setTile(pos, getAutoTileIndexForTileCheck(getMap()->checkAutoTiles(pair.second.m_uiAutoTileSetID, pos, positions, m_Layer)), m_Layer);
        std::for_each(positions.cbegin(), positions.cend(), [this, pos](UInt32Point curPos)
        {
            UInt32Point3D pos(curPos, pos.z);
            uint32 uiCurAutoTileID = getMap()->getAutoTile(pos, m_Layer);
            if (uiCurAutoTileID)
                getMap()->setTile(pos, getAutoTileIndexForTileCheck(getMap()->checkAutoTiles(uiCurAutoTileID, pos,
                UInt32PointSet(), m_Layer, MapPrototype::FLAG_NOTHING)), this->m_Layer);
        });
    }
}