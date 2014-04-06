#include "Brush.h"

using namespace MAP;
using namespace BRUSH;

/*#####
# Brush
#####*/
Brush::Brush(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, uint32 ID, SelectionType selectionType, LayerType layerType, uint32 layerIndex) :
    m_ID(ID), m_SelectionType(selectionType), m_DBMgr(DBMgr), m_LayerIndex(layerIndex), m_LayerType(layerType), m_MapLayer(mapLayer),
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
        uint32 borderCheck = m_MapLayer.checkAutoTiles(getID(), tilePosition, positionChecks, getLayerType(), LayerContainer::FLAG_ALL);
        auto& mapTile = m_MapLayer.getMapTile(tilePosition, getLayerType());
        if (mapTile.isAutoTile())
            mapTile.m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(borderCheck);
    }

    // update tiles
    for (auto& tilePos : positionChecks)
    {
        // is position check?
        if (checkPositions.get(tilePos))
            continue;
        checkPositions.set(tilePos);

        UInt32Point3D pos(tilePos, getLayerIndex());
        auto& tile = m_MapLayer.getMapTile(pos, getLayerType());
        if (!tile.isAutoTile())
            continue;

        // store tile
        m_RevertInfo.addTile(tilePos, tile);

        tile.m_uiTileID = DATABASE::AUTO_TILE::getAutoTileIndexForTileCheck(m_MapLayer.checkAutoTiles(tile.m_uiAutoTileSetID, pos, UInt32PointVector(),
            getLayerType(), LayerContainer::FLAG_NOTHING));
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
BrushFill::BrushFill(const DATABASE::DatabaseMgr& DBMgr, LayerContainer &mapLayer, uint32 ID, SelectionType selectionType, LayerType layerType, uint32 layerIndex)
    : Brush(DBMgr, mapLayer, ID, selectionType, layerType, layerIndex)
{
}

bool BrushFill::_getPosition(uint32 i, UInt32Point3D &checkPos) const
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
    MAP::MapTile startTile(m_MapLayer.getMapTile(UInt32Point3D(center, getLayerIndex()), getLayerType()));
    if (!startTile.isValid())
        return;

    // store center in open points
    UInt32PointList openPoints;
    openPoints.push_back(center);
    checkPositions.set(center);

    while (!openPoints.empty())
    {
        UInt32Point3D openPointPos(openPoints.front(), getLayerIndex());
        openPoints.pop_front();

        // store positions around
        for (uint32 i = 0; i < 4; ++i)
        {
            // set position check
            UInt32Point3D checkPos = openPointPos;
            if (!_getPosition(i, checkPos))
                continue;

            const auto& checkTile = m_MapLayer.getMapTile(UInt32Point3D(checkPos, getLayerIndex()), getLayerType());
            if (checkTile == startTile)
            {
                // bitset check
                if (checkPositions.get(checkPos))
                    continue;
                checkPositions.set(checkPos);
                openPoints.push_back(checkPos);
            }
        }

        // setup revert
        m_RevertInfo.addTile(openPointPos, m_MapLayer.getMapTile(openPointPos, getLayerType()));
        
        m_MapLayer.setMapTile(openPointPos, getLayerType(), MapTile(getID(), 0));
    }
}

void BrushFill::_drawAutoTile(const UInt32Point& center)
{
    Bitset2D checkPositions;
    MAP::MapTile startTile(m_MapLayer.getMapTile(UInt32Point3D(center, getLayerIndex()), getLayerType()));
    if (!startTile.isValid())
        return;

    // store center in open points
    UInt32PointList openPoints;
    openPoints.push_back(center);
    checkPositions.set(center);

    UInt32PointVector borderChecks;
    while (!openPoints.empty())
    {
        UInt32Point3D openPointPos(openPoints.front(), getLayerIndex());
        openPoints.pop_front();

        // store positions around
        bool pushIntoBorderCheck = false;
        for (uint32 i = 0; i < 4; ++i)
        {
            // set position check
            UInt32Point3D checkPos = openPointPos;
            if (!_getPosition(i, checkPos))
                continue;

            const auto& checkTile = m_MapLayer.getMapTile(UInt32Point3D(checkPos, getLayerIndex()), getLayerType());
            if (checkTile == startTile)
            {
                // bitset check
                if (checkPositions.get(checkPos))
                    continue;
                checkPositions.set(checkPos);
                openPoints.push_back(checkPos);
            }
            else if (checkTile.m_uiAutoTileSetID != getID() || !checkPositions.get(checkPos) && checkTile.m_uiAutoTileSetID == getID())
                pushIntoBorderCheck = true;
        }

        if (pushIntoBorderCheck)
            borderChecks.push_back(openPointPos);

        // setup revert
        m_RevertInfo.addTile(openPointPos, m_MapLayer.getMapTile(openPointPos, getLayerType()));

        m_MapLayer.setMapTile(openPointPos, getLayerType(), MapTile(DATABASE::AUTO_TILE::INDEX_CENTER, getID()));
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
    MAP::MapTile startTile(m_MapLayer.getMapTile(UInt32Point3D(center, getLayerIndex()), getLayerType()));
    if (!startTile.isValid())
        return;

    // store center in open points
    UInt32PointVector openPoints;
    openPoints.push_back(center);
    checkPositions.set(center);

    while (!openPoints.empty())
    {
        UInt32Point3D openPointPos(openPoints.back(), getLayerIndex());
        openPoints.pop_back();

        for (uint32 i = 0; i < 4; ++i)
        {
            // set position check
            UInt32Point3D checkPos = openPointPos;
            if (!_getPosition(i, checkPos))
                continue;

            const auto& checkTile = m_MapLayer.getMapTile(UInt32Point3D(checkPos, getLayerIndex()), getLayerType());
            if (checkTile == startTile)
            {
                // bitset check
                if (checkPositions.get(checkPos))
                    continue;
                checkPositions.set(checkPos);
                openPoints.push_back(checkPos);
            }
        }

        // setup revert
        m_RevertInfo.addTile(openPointPos, m_MapLayer.getMapTile(openPointPos, getLayerType()));

        // get correct tile ID
        UInt32Point index(openPointPos.x % tileSetSize.x - center.x % tileSetSize.x, openPointPos.y % tileSetSize.y - center.y % tileSetSize.y);
        // if index overflows, add tileCount
        if (index.x > tileSetSize.x)
            index.x += tileSetSize.x;
        if (index.y > tileSetSize.y)
            index.y += tileSetSize.y;
        m_MapLayer.setMapTile(openPointPos, getLayerType(), MapTile(pTileSet->getTileID(index), 0));
    }
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
        _drawTile(pos);
        break;
    case MAP::BRUSH::SelectionType::AUTO_TILES:
        {
            // is same tile?
            const auto& tile = m_MapLayer.getMapTile(UInt32Point3D(pos, getLayerIndex()), getLayerType());
            if (tile.isAutoTile() && tile.m_uiAutoTileSetID == getID())
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
