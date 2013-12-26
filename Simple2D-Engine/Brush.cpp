#include "Brush.h"

using namespace MAP;
using namespace BRUSH;

/*#####
# Brush
#####*/
Brush::Brush(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, uint32 ID, SelectionType selectionType, Layer layerType, uint32 layerIndex) :
    m_ID(ID), m_SelectionType(selectionType), m_DBMgr(DBMgr), m_LayerIndex(layerIndex), m_LayerType(layerType), m_MapLayer(mapLayer),
    m_RevertInfo(mapLayer.getSize(), layerIndex, layerType)
{
}

void Brush::updateAutoTilesAround(const UInt32PointVector& positions)
{
    BitsetVector checkPositions(m_MapLayer.getSize().y, boost::dynamic_bitset<>(m_MapLayer.getSize().x));

    // get tiles
    UInt32PointVector positionChecks;
    for (auto current : positions)
    {
        checkPositions.at(current.y)[current.x] = true;
        auto tilePosition = UInt32Point3D(current, getLayerIndex());
        uint32 borderCheck = m_MapLayer.checkAutoTiles(getID(), tilePosition, positionChecks, getLayerType(), MapLayer::FLAG_ALL);
        auto& mapTile = m_MapLayer.getMapTile(tilePosition, getLayerType());
        if (mapTile.isAutoTile())
            mapTile.m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(borderCheck);
    }

    // update tiles
    for (auto& tilePos : positionChecks)
    {
        // is position check?
        if (checkPositions.at(tilePos.y)[tilePos.x])
            continue;
        checkPositions.at(tilePos.y)[tilePos.x] = true;

        UInt32Point3D pos(tilePos, getLayerIndex());
        auto& tile = m_MapLayer.getMapTile(pos, getLayerType());
        if (!tile.isAutoTile())
            continue;

        // store tile
        m_RevertInfo.addTile(tilePos, tile);

        tile.m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(m_MapLayer.checkAutoTiles(tile.m_uiAutoTileSetID, pos, UInt32PointVector(),
            getLayerType(), MapLayer::FLAG_NOTHING));
    }
}

/*#####
# BrushPen
#####*/
BrushPen::BrushPen(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, uint32 ID, SelectionType selectionType, Layer layerType, uint32 layerIndex) :
    Brush(DBMgr, mapLayer, ID, selectionType, layerType, layerIndex)
{
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
    m_RevertInfo.addTile(pos, oldTile);

    m_MapLayer.setMapTile(currentPos, getLayerType(), newMapTile);
    UInt32PointVector positions;
    positions.push_back(pos);
    updateAutoTilesAround(positions);
}

void BrushPen::_drawAutoTile(const UInt32Point& pos)
{
    UInt32Point3D currentPos(pos, getLayerIndex());
    // when same auto tile, return
    auto& tile = m_MapLayer.getMapTile(currentPos, getLayerType());
    if (tile.m_uiAutoTileSetID == getID())
        return;

    // store old tile in revert info
    m_RevertInfo.addTile(pos, tile);

    // setup autotile
    tile.m_uiAutoTileSetID = getID();

    // check around
    UInt32PointVector positions;
    positions.push_back(pos);
    updateAutoTilesAround(positions);
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

                auto currentPos3D = UInt32Point3D(currentPos, getLayerIndex());

                // store old tile in revert info
                m_RevertInfo.addTile(currentPos, m_MapLayer.getMapTile(currentPos3D, getLayerType()));

                // setup tile
                m_MapLayer.setMapTile(currentPos3D, getLayerType(), MapTile(pTileSet->getTileID(tilePos), 0));
            }
        }

        // update around
        updateAutoTilesAround(positions);
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

void BrushFill::_updateAutoTilesAround(const UInt32PointVector& positions)
{
}

void BrushFill::_drawTile(const UInt32Point& pos)
{
    //// store old tile in revert info
    //m_RevertInfo.m_Tiles.insert(std::make_pair(pos, oldTile));

    //m_MapLayer.setMapTile(currentPos, getLayerType(), newMapTile);
    //UInt32PointVector positions;
    //positions.push_back(pos);
    //_updateAutoTilesAround(positions);
}

void BrushFill::_drawAutoTile(const UInt32Point& pos)
{
}

void BrushFill::_drawTileSet(const UInt32Point& pos)
{
}

void BrushFill::draw(const UInt32Point& pos)
{
    if (!m_MapLayer.isInMap(pos) || m_MapLayer.getLayerSize(getLayerType()) <= getLayerIndex())
        return;

    switch (getSelectionType())
    {
    case MAP::BRUSH::SelectionType::TILES:
        // is same tile?
        if (m_MapLayer.getMapTile(UInt32Point3D(pos, getLayerIndex()), getLayerType()) == MapTile(getID(), 0))
            return;
        break;
    case MAP::BRUSH::SelectionType::AUTO_TILES:
        {
            // is same tile?
            const auto& tile = m_MapLayer.getMapTile(UInt32Point3D(pos, getLayerIndex()), getLayerType());
            if (tile.isAutoTile() && tile.m_uiAutoTileSetID == getID())
                return;
            break;
        }
    case MAP::BRUSH::SelectionType::TILE_SETS: _drawTileSet(pos); break;
    }

    _calculateFillArea(pos);
}

void BrushFill::_calculateFillArea(const UInt32Point &center)
{
    BitsetVector checkPositions(m_MapLayer.getSize().y, boost::dynamic_bitset<>(m_MapLayer.getSize().x));

    MAP::MapTile startTile(m_MapLayer.getMapTile(UInt32Point3D(center, getLayerIndex()), getLayerType()));
    if (!startTile.isValid())
        return;

    // store center in open points
    UInt32PointList openPoints;
    openPoints.push_back(center);
    checkPositions.at(center.x)[center.y] = true;

    UInt32PointVector borderChecks;
    while (!openPoints.empty())
    {
        UInt32Point3D openPointPos(openPoints.front(), getLayerIndex());
        openPoints.pop_front();

        // store positions around
        bool pushIntoBorderCheck = false;
        for (uint32 i = 0; i < 4; ++i)
        {
            DATABASE::AUTO_TILE::TILE_CHECK curTileCheck = DATABASE::AUTO_TILE::SAME_AROUND;
            // set position check
            UInt32Point3D checkPos = openPointPos;
            switch (i)
            {
            case 0: // top
                if (!checkPos.y)
                    continue;
                --checkPos.y;
                curTileCheck = DATABASE::AUTO_TILE::OTHER_TOP;
                break;

            case 1: // left
                if (!checkPos.x)
                    continue;
                --checkPos.x;
                curTileCheck = DATABASE::AUTO_TILE::OTHER_LEFT;
                break;

            case 2: // right
                if (checkPos.x+1 >= m_MapLayer.getSize().x)
                    continue;
                ++checkPos.x;
                curTileCheck = DATABASE::AUTO_TILE::OTHER_RIGHT;
                break;

            case 3: // bottom
                if (checkPos.y+1 >= m_MapLayer.getSize().y)
                    continue;
                ++checkPos.y;
                curTileCheck = DATABASE::AUTO_TILE::OTHER_BOTTOM;
                break;
            }

            const auto& checkTile = m_MapLayer.getMapTile(UInt32Point3D(checkPos, getLayerIndex()), getLayerType());
            if (!checkTile.isAutoTile() && checkTile == startTile ||
                checkTile.isAutoTile() && checkTile.m_uiAutoTileSetID == startTile.m_uiAutoTileSetID)
            {
                // bitset check
                if (checkPositions.at(checkPos.x)[checkPos.y])
                    continue;
                checkPositions.at(checkPos.x)[checkPos.y] = true;

                openPoints.push_back(checkPos);
            }
            else if (checkTile.m_uiAutoTileSetID != getID() || !checkPositions.at(checkPos.x)[checkPos.y] && checkTile.m_uiAutoTileSetID == getID())
                pushIntoBorderCheck = true;
        }

        if (pushIntoBorderCheck)
            borderChecks.push_back(openPointPos);

        // setup revert
        m_RevertInfo.addTile(openPointPos, m_MapLayer.getMapTile(openPointPos, getLayerType()));

        switch (getSelectionType())
        {
        case MAP::BRUSH::SelectionType::TILES: m_MapLayer.setMapTile(openPointPos, getLayerType(), MapTile(getID(), 0)); break;
        case MAP::BRUSH::SelectionType::AUTO_TILES:
            m_MapLayer.setMapTile(openPointPos, getLayerType(), MapTile(DATABASE::AUTO_TILE::INDEX_CENTER, getID()));
            break;
        case MAP::BRUSH::SelectionType::TILE_SETS:break;
        }
    }

    // do final checks
    if (getSelectionType() == MAP::BRUSH::SelectionType::AUTO_TILES)
        updateAutoTilesAround(borderChecks);
}

/*#####
# BrushFactory
#####*/
BrushPtr BrushFactory::createBrush(const DATABASE::DatabaseMgr& DBMgr, MapLayer &mapLayer, const BrushInfo& info, Layer layerType, uint32 layerIndex)
{
    switch (info.m_BrushType)
    {
    case BrushType::PEN: return BrushPtr(new BrushPen(DBMgr, mapLayer, info.m_ID, info.m_SelectionType, layerType, layerIndex)); break;
    case BrushType::FILL: return BrushPtr(new BrushFill(DBMgr, mapLayer, info.m_ID, info.m_SelectionType, layerType, layerIndex)); break;
    }
    throw std::bad_typeid("invalid brush type");
}
