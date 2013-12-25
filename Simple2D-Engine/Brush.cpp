#include "Brush.h"

using namespace MAP;
using namespace BRUSH;

/*#####
# Brush
#####*/
Brush::Brush(const DATABASE::DatabaseMgr& DBMgr, uint32 ID, SelectionType selectionType) : m_ID(ID), m_SelectionType(selectionType), m_DBMgr(DBMgr)
{
}

/*#####
# BrushPen
#####*/
BrushPen::BrushPen(const DATABASE::DatabaseMgr& DBMgr, uint32 ID, SelectionType selectionType) : Brush(DBMgr, ID, selectionType)
{
}

void BrushPen::_updateAutoTilesAround(MapLayer& mapLayer, const UInt32PointVector& positions, uint32 layerIndex, Layer layerType) const
{
    // get tiles
    UInt32PointUSet positionChecks;
    for (auto current : positions)
        mapLayer.checkAutoTiles(0, UInt32Point3D(current, layerIndex), positionChecks, layerType, MapLayer::FLAG_OTHER);

    // update tiles
    for (auto& tilePos : positionChecks)
    {
        UInt32Point3D pos(tilePos, layerIndex);
        auto& tile = mapLayer.getMapTile(pos, layerType);
        tile.m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(mapLayer.checkAutoTiles(tile.m_uiAutoTileSetID, pos, UInt32PointUSet(),
            layerType, MapLayer::FLAG_NOTHING));
    }
}

void BrushPen::_drawTile( MapLayer &mapLayer, const UInt32Point3D &pos, Layer layerType ) const
{
    // check center
    MapTile newMapTile(getID(), 0);
    if (mapLayer.getMapTile(pos, layerType) == newMapTile)
        return;

    mapLayer.setMapTile(pos, layerType, newMapTile);
    UInt32PointVector positions;
    positions.push_back(pos);
    _updateAutoTilesAround(mapLayer, positions, pos.z, layerType);
}

void BrushPen::_drawAutoTile( MapLayer &mapLayer, const UInt32Point3D &pos, Layer layerType ) const
{
    // when same auto tile, return
    auto& tile = mapLayer.getMapTile(pos, layerType);
    if (tile.m_uiAutoTileSetID == getID())
        return;

    // setup autotile
    tile.m_uiAutoTileSetID = getID();
    tile.m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(mapLayer.checkAutoTiles(tile.m_uiAutoTileSetID, pos, UInt32PointUSet(),
        layerType, MapLayer::FLAG_NOTHING));

    // check around
    UInt32PointVector positions;
    positions.push_back(pos);
    _updateAutoTilesAround(mapLayer, positions, pos.z, layerType);
}

void BrushPen::_drawTileSet( const UInt32Point3D &pos, MapLayer &mapLayer, Layer layerType ) const
{
    if (auto pTileSet = m_DBMgr.getTileSetDatabase()->getOriginalPrototype(getID()))
    {
        UInt32PointVector positions;
        UInt32Point tilePos;
        for (tilePos.x = 0; tilePos.x < pTileSet->getTileCount().x; ++tilePos.x)
        {
            for (tilePos.y = 0; tilePos.y < pTileSet->getTileCount().y; ++tilePos.y)
            {
                UInt32Point3D currentPos = pos + UInt32Point3D(tilePos);
                if (!mapLayer.isInMap(currentPos))
                    continue;

                // store position
                positions.push_back(currentPos);

                // setup tile
                mapLayer.setMapTile(currentPos, layerType, MapTile(pTileSet->getTileID(tilePos), 0));
            }
        }

        // update around
        _updateAutoTilesAround(mapLayer, positions, pos.z, layerType);
    }
}

void BrushPen::draw(MapLayer& mapLayer, const UInt32Point3D& pos, Layer layerType) const
{
    if (!mapLayer.isInMap(pos) || mapLayer.getLayerSize(layerType) <= pos.z)
        return;

    switch (getSelectionType())
    {
    case MAP::BRUSH::SelectionType::TILES: _drawTile(mapLayer, pos, layerType); break;
    case MAP::BRUSH::SelectionType::AUTO_TILES: _drawAutoTile(mapLayer, pos, layerType); break;
    case MAP::BRUSH::SelectionType::TILE_SETS: _drawTileSet(pos, mapLayer, layerType); break;
    }
}

/*#####
# BrushFill
#####*/
BrushFill::BrushFill(const DATABASE::DatabaseMgr& DBMgr, uint32 ID, SelectionType selectionType) : Brush(DBMgr, ID, selectionType)
{
}

void BrushFill::draw(MapLayer& mapLayer, const UInt32Point3D& pos, Layer layerType) const
{
}

/*#####
# BrushFactory
#####*/
BrushPtr BrushFactory::createBrush(const DATABASE::DatabaseMgr& DBMgr, const BrushInfo& info)
{
    switch (info.m_BrushType)
    {
    case BrushType::PEN: return BrushPtr(new BrushPen(DBMgr, info.m_ID, info.m_SelectionType)); break;
    case BrushType::FILL: return BrushPtr(new BrushPen(DBMgr, info.m_ID, info.m_SelectionType)); break;
    }
    throw std::bad_typeid("invalid brush type");
}
