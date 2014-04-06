#include "Brush.h"

using namespace MAP;
using namespace BRUSH;

/*#####
# Brush
#####*/
Brush::Brush(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, uint32 ID, SelectionType selectionType, LayerType layerType, uint32 layerIndex) :
    m_ID(ID), m_SelectionType(selectionType), m_DBMgr(DBMgr), m_LayerIndex(layerIndex), m_LayerType(layerType), m_MapLayer(mapLayer.getLayer(layerType, layerIndex)),
    m_RevertInfo(layerType, layerIndex)
{
}

void Brush::updateAutoTilesAround(const UInt32PointVector& positions)
{
    Bitset2D checkPositions;

    // get tiles
    UInt32PointVector positionChecks;
    for (auto current : positions)
    {
        checkPositions.set(current);
        auto tilePosition = UInt32Point3D(current, getLayerIndex());
        uint32 borderCheck = m_MapLayer.checkAutoTiles(getID(), tilePosition, positionChecks, Layer::FLAG_ALL);
        auto& mapTile = m_MapLayer.getMapTile(tilePosition);
        if (mapTile.getMapTile().isAutoTile())
            mapTile.getMapTile().m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(borderCheck);
    }

    // update tiles
    for (auto& tilePos : positionChecks)
    {
        // is position check?
        if (checkPositions.get(tilePos))
            continue;
        checkPositions.set(tilePos);

        UInt32Point3D pos(tilePos, getLayerIndex());
        auto& tile = m_MapLayer.getMapTile(pos);
        if (!tile.getMapTile().isAutoTile())
            continue;

        // store tile
        m_RevertInfo.addTile(tilePos, tile.getMapTile());

        tile.getMapTile().m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(m_MapLayer.checkAutoTiles(tile.getMapTile().m_uiAutoTileSetID, pos, UInt32PointVector(),
            Layer::FLAG_NOTHING));
    }
}

/*#####
# BrushPen
#####*/
BrushPen::BrushPen(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, uint32 ID, SelectionType selectionType, LayerType layerType, uint32 layerIndex) :
    Brush(DBMgr, mapLayer, ID, selectionType, layerType, layerIndex)
{
}

void BrushPen::_drawTile(const UInt32Point& pos)
{

    // check center
    MapTile newMapTile(getID(), 0);
    auto oldTile = m_MapLayer.getMapTile(pos);
    if (oldTile.getMapTile() == newMapTile)
        return;

    // store old tile in revert info
    m_RevertInfo.addTile(pos, oldTile.getMapTile());

    m_MapLayer.setMapTile(pos, newMapTile);
    UInt32PointVector positions;
    positions.push_back(pos);
    updateAutoTilesAround(positions);
}

void BrushPen::_drawAutoTile(const UInt32Point& pos)
{
    // when same auto tile, return
    auto& tile = m_MapLayer.getMapTile(pos);
    if (tile.getMapTile().m_uiAutoTileSetID == getID())
        return;

    // store old tile in revert info
    m_RevertInfo.addTile(pos, tile.getMapTile());

    // setup autotile
    tile.getMapTile().m_uiAutoTileSetID = getID();

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

                // store old tile in revert info
                m_RevertInfo.addTile(currentPos, m_MapLayer.getMapTile(currentPos).getMapTile());

                // setup tile
                m_MapLayer.setMapTile(currentPos, MapTile(pTileSet->getTileID(tilePos), 0));
            }
        }

        // update around
        updateAutoTilesAround(positions);
    }
}

void BrushPen::draw(const UInt32Point& pos)
{
    if (!m_MapLayer.isInMap(pos))
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
BrushFill::BrushFill(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, uint32 ID, SelectionType selectionType, LayerType layerType, uint32 layerIndex)
    : Brush(DBMgr, mapLayer, ID, selectionType, layerType, layerIndex)
{
}

bool BrushFill::_getPosition(uint32 i, UInt32Point &checkPos) const
{
    switch (i)
    {
    case 0: // top
        if (!checkPos.y)
            return false;
        --checkPos.y;
        break;

    case 1: // left
        if (!checkPos.x)
            return false;
        --checkPos.x;
        break;

    case 2: // right
        if (checkPos.x+1 >= m_MapLayer.getSize().x)
            return false;
        ++checkPos.x;
        break;

    case 3: // bottom
        if (checkPos.y+1 >= m_MapLayer.getSize().y)
            return false;
        ++checkPos.y;
        break;
    }
    return true;
}

void BrushFill::_drawTile(const UInt32Point& center)
{
    Bitset2D checkPositions;
    auto startTile(m_MapLayer.getMapTile(center));
    if (!startTile.getMapTile().isValid())
        return;

    // store center in open points
    UInt32PointList openPoints;
    openPoints.push_back(center);
    checkPositions.set(center);

    while (!openPoints.empty())
    {
        UInt32Point openPointPos(openPoints.front());
        openPoints.pop_front();

        // store positions around
        for (uint32 i = 0; i < 4; ++i)
        {
            // set position check
            UInt32Point checkPos = openPointPos;
            if (!_getPosition(i, checkPos))
                continue;

            auto checkTile = m_MapLayer.getMapTile(checkPos);
            if (checkTile.getMapTile() == startTile.getMapTile())
            {
                // bitset check
                if (checkPositions.get(checkPos))
                    continue;
                checkPositions.set(checkPos);
                openPoints.push_back(checkPos);
            }
        }

        // setup revert
        auto tileInfo = m_MapLayer.getMapTile(openPointPos);
        m_RevertInfo.addTile(openPointPos, tileInfo.getMapTile());
        m_MapLayer.setMapTile(openPointPos, MapTile(getID(), 0));
    }
}

void BrushFill::_drawAutoTile(const UInt32Point& center)
{
    Bitset2D checkPositions;
    auto startTile(m_MapLayer.getMapTile(center));
    if (!startTile.getMapTile().isValid())
        return;

    // store center in open points
    UInt32PointList openPoints;
    openPoints.push_back(center);
    checkPositions.set(center);

    UInt32PointVector borderChecks;
    while (!openPoints.empty())
    {
        UInt32Point openPointPos(openPoints.front());
        openPoints.pop_front();

        // store positions around
        bool pushIntoBorderCheck = false;
        for (uint32 i = 0; i < 4; ++i)
        {
            // set position check
            UInt32Point checkPos = openPointPos;
            if (!_getPosition(i, checkPos))
                continue;

            auto checkTile = m_MapLayer.getMapTile(checkPos);
            if (checkTile.getMapTile() == startTile.getMapTile())
            {
                // bitset check
                if (checkPositions.get(checkPos))
                    continue;
                checkPositions.set(checkPos);
                openPoints.push_back(checkPos);
            }
            else if (checkTile.getMapTile().m_uiAutoTileSetID != getID() ||
                !checkPositions.get(checkPos) && checkTile.getMapTile().m_uiAutoTileSetID == getID())
                pushIntoBorderCheck = true;
        }

        if (pushIntoBorderCheck)
            borderChecks.push_back(openPointPos);

        // setup revert
        auto tileInfo = m_MapLayer.getMapTile(openPointPos);
        m_RevertInfo.addTile(openPointPos, tileInfo.getMapTile());
        m_MapLayer.setMapTile(openPointPos, MapTile(DATABASE::AUTO_TILE::INDEX_CENTER, getID()));
    }

    // do final checks
    if (getSelectionType() == MAP::BRUSH::SelectionType::AUTO_TILES)
        updateAutoTilesAround(borderChecks);
}

void BrushFill::_drawTileSet(const UInt32Point& center)
{
    auto pTileSet = m_DBMgr.getTileSetDatabase()->getOriginalPrototype(getID());
    if (!pTileSet)
        return;

    const auto tileSetSize = pTileSet->getTileCount();
    Bitset2D checkPositions;
    auto startTile(m_MapLayer.getMapTile(center));
    if (!startTile.getMapTile().isValid())
        return;

    // store center in open points
    UInt32PointVector openPoints;
    openPoints.push_back(center);
    checkPositions.set(center);

    while (!openPoints.empty())
    {
        UInt32Point openPointPos(openPoints.back());
        openPoints.pop_back();

        for (uint32 i = 0; i < 4; ++i)
        {
            // set position check
            UInt32Point checkPos = openPointPos;
            if (!_getPosition(i, checkPos))
                continue;

            auto checkTile = m_MapLayer.getMapTile(checkPos);
            if (checkTile.getMapTile() == startTile.getMapTile())
            {
                // bitset check
                if (checkPositions.get(checkPos))
                    continue;
                checkPositions.set(checkPos);
                openPoints.push_back(checkPos);
            }
        }

        // setup revert
        auto tileInfo = m_MapLayer.getMapTile(openPointPos);
        m_RevertInfo.addTile(openPointPos, tileInfo.getMapTile());

        // get correct tile ID
        UInt32Point index(openPointPos.x % tileSetSize.x - center.x % tileSetSize.x, openPointPos.y % tileSetSize.y - center.y % tileSetSize.y);
        // if index overflows, add tileCount
        if (index.x > tileSetSize.x)
            index.x += tileSetSize.x;
        if (index.y > tileSetSize.y)
            index.y += tileSetSize.y;
        m_MapLayer.setMapTile(openPointPos, MapTile(pTileSet->getTileID(index), 0));
    }
}

void BrushFill::draw(const UInt32Point& pos)
{
    if (!m_MapLayer.isInMap(pos))
        return;

    switch (getSelectionType())
    {
    case MAP::BRUSH::SelectionType::TILES:
        {
            // is same tile?
            auto tileInfo = m_MapLayer.getMapTile(pos);
            if (tileInfo.getMapTile() == MapTile(getID(), 0))
                return;
            _drawTile(pos);
            break;
        }
    case MAP::BRUSH::SelectionType::AUTO_TILES:
        {
            // is same tile?
            auto tile = m_MapLayer.getMapTile(pos);
            if (tile.getMapTile().isAutoTile() && tile.getMapTile().m_uiAutoTileSetID == getID())
                return;
            _drawAutoTile(pos);
            break;
        }
    case MAP::BRUSH::SelectionType::TILE_SETS: _drawTileSet(pos); break;
    }
}

/*#####
# BrushFactory
#####*/
BrushPtr BrushFactory::createBrush(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, const BrushInfo& info, LayerType layerType, uint32 layerIndex)
{
    switch (info.m_BrushType)
    {
    case BrushType::PEN: return BrushPtr(new BrushPen(DBMgr, mapLayer, info.m_ID, info.m_SelectionType, layerType, layerIndex)); break;
    case BrushType::FILL: return BrushPtr(new BrushFill(DBMgr, mapLayer, info.m_ID, info.m_SelectionType, layerType, layerIndex)); break;
    }
    throw std::bad_typeid("invalid brush type");
}
