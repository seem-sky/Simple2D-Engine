#include "MapAction.h"

using namespace MAP;

/*#####
# MapAction
#####*/
MapAction::MapAction(const MapPrototypePtr &map) : m_pMap(map)
{
}

/*#####
# TileMapAction
#####*/
TileMapAction::TileMapAction(const Point3D<uint32> &pos, const uint32 &uiTileID, const MapPrototypePtr &map) : MapAction(map), m_Pos(pos), m_uiTileID(uiTileID)
{
}

void TileMapAction::revertMapAction()
{
    if (!getMap())
        return;

    getMap()->setTile(m_Pos, m_uiTileID);
}

/*#####
# TileMapAction
#####*/
MultiTileMapAction::MultiTileMapAction(const uint32 &uiTileID, const MapPrototypePtr &map) : MapAction(map), m_uiTileID(uiTileID)
{
}

void MultiTileMapAction::revertMapAction()
{
    if (!getMap())
        return;

    for (uint32 i = m_Positions.size(); i > 0; --i)
        getMap()->setTile(m_Positions.at(i-1), m_uiTileID);
}

void MultiTileMapAction::addPosition(const Point3D<uint32> &pos)
{
    m_Positions.push_back(pos);
}