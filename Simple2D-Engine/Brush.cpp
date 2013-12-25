#include "Brush.h"

using namespace MAP;
using namespace BRUSH;

/*#####
# Brush
#####*/
Brush::Brush(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, uint32 ID, SelectionType selectionType, Layer layerType, uint32 layerIndex) :
    m_ID(ID), m_SelectionType(selectionType), m_DBMgr(DBMgr), m_LayerIndex(layerIndex), m_LayerType(layerType), m_MapLayer(mapLayer)
{
    m_RevertInfo.m_LayerIndex = m_LayerIndex;
    m_RevertInfo.m_LayerType = m_LayerType;
}

/*#####
# BrushPen
#####*/
BrushPen::BrushPen(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, uint32 ID, SelectionType selectionType, Layer layerType, uint32 layerIndex) :
    Brush(DBMgr, mapLayer, ID, selectionType, layerType, layerIndex)
{
}

void BrushPen::_updateAutoTilesAround(const UInt32PointVector& positions)
{
    // get tiles
    UInt32PointUSet positionChecks;
    for (auto current : positions)
        m_MapLayer.checkAutoTiles(0, UInt32Point3D(current, getLayerIndex()), positionChecks, getLayerType(), MapLayer::FLAG_OTHER);

    // update tiles
    for (auto& tilePos : positionChecks)
    {
        UInt32Point3D pos(tilePos, getLayerIndex());
        auto& tile = m_MapLayer.getMapTile(pos, getLayerType());
        tile.m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(m_MapLayer.checkAutoTiles(tile.m_uiAutoTileSetID, pos, UInt32PointUSet(),
            getLayerType(), MapLayer::FLAG_NOTHING));
    }
}

void BrushPen::_drawTile(const UInt32Point& pos)
{
    // check center
    MapTile newMapTile(getID(), 0);
    UInt32Point3D currentPos(pos, getLayerIndex());
    auto oldTile = m_MapLayer.getMapTile(currentPos, getLayerType());
    if (oldTile == newMapTile)
        return;

    // store old tile in revert info
    m_RevertInfo.m_Tiles.push_back(std::make_pair(oldTile, pos));

    m_MapLayer.setMapTile(currentPos, getLayerType(), newMapTile);
    UInt32PointVector positions;
    positions.push_back(pos);
    _updateAutoTilesAround(positions);
}

void BrushPen::_drawAutoTile(const UInt32Point& pos)
{
    UInt32Point3D currentPos(pos, getLayerIndex());
    // when same auto tile, return
    auto& tile = m_MapLayer.getMapTile(currentPos, getLayerType());
    if (tile.m_uiAutoTileSetID == getID())
        return;

    // setup autotile
    tile.m_uiAutoTileSetID = getID();
    tile.m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(m_MapLayer.checkAutoTiles(tile.m_uiAutoTileSetID, currentPos, UInt32PointUSet(),
        getLayerType(), MapLayer::FLAG_NOTHING));

    // check around
    UInt32PointVector positions;
    positions.push_back(pos);
    _updateAutoTilesAround(positions);
}

void BrushPen::_drawTileSet(const UInt32Point& pos)
{
    if (auto pTileSet = m_DBMgr.getTileSetDatabase()->getOriginalPrototype(getID()))
    {
        UInt32PointVector positions;
        UInt32Point tilePos;
        for (tilePos.x = 0; tilePos.x < pTileSet->getTileCount().x; ++tilePos.x)
        {
            for (tilePos.y = 0; tilePos.y < pTileSet->getTileCount().y; ++tilePos.y)
            {
                UInt32Point currentPos = pos + tilePos;
                if (!m_MapLayer.isInMap(currentPos))
                    continue;

                // store position
                positions.push_back(currentPos);

                // setup tile
                m_MapLayer.setMapTile(UInt32Point3D(currentPos, getLayerIndex()), getLayerType(), MapTile(pTileSet->getTileID(tilePos), 0));
            }
        }

        // update around
        _updateAutoTilesAround(positions);
    }
}

void BrushPen::draw(const UInt32Point& pos)
{
    if (!m_MapLayer.isInMap(pos) || m_MapLayer.getLayerSize(getLayerType()) <= getLayerIndex())
        return;

    switch (getSelectionType())
    {
    case MAP::BRUSH::SelectionType::TILES: _drawTile(pos); break;
    case MAP::BRUSH::SelectionType::AUTO_TILES: _drawAutoTile(pos); break;
    case MAP::BRUSH::SelectionType::TILE_SETS: _drawTileSet(pos); break;
    }
}

/*#####
# BrushFill
#####*/
BrushFill::BrushFill(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, uint32 ID, SelectionType selectionType, Layer layerType, uint32 layerIndex)
    : Brush(DBMgr, mapLayer, ID, selectionType, layerType, layerIndex)
{
}

void BrushFill::draw(const UInt32Point& pos)
{
}

/*#####
# BrushFactory
#####*/
BrushPtr BrushFactory::createBrush(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, const BrushInfo& info, Layer layerType, uint32 layerIndex)
{
    switch (info.m_BrushType)
    {
    case BrushType::PEN: return BrushPtr(new BrushPen(DBMgr, mapLayer, info.m_ID, info.m_SelectionType, layerType, layerIndex)); break;
    case BrushType::FILL: return BrushPtr(new BrushPen(DBMgr, mapLayer, info.m_ID, info.m_SelectionType, layerType, layerIndex)); break;
    }
    throw std::bad_typeid("invalid brush type");
}
