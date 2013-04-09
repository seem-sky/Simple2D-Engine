#include "MapBrush.h"
#include <QtGui/QPainter>
#include <QtGui/QBitmap>
#include "Config.h"

using namespace MAP;
using namespace DATABASE;

/*#####
# MapBrush
#####*/

MapBrush::MapBrush() : m_buttonHold(false), m_pLastEmitter(NULL), m_Type(BRUSH_PEN), m_uiTileID(0)
{}

void MapBrush::brushPress(MapViewWidget *pWidget, Point3D<uint32> center)
{
    m_pLastEmitter = pWidget;
    m_buttonHold = true;
    drawOnMap(pWidget, center);
}

void MapBrush::brushRelease(MapViewWidget *pWidget, Point3D<uint32> center)
{
    if (m_pLastEmitter != pWidget || !m_buttonHold)
        return;

    m_pLastEmitter = pWidget;
    m_buttonHold = false;
    switch (m_Type)
    {
    case BRUSH_RECT: drawOnMap(pWidget, center); break;
    }
}

void MapBrush::brushMove(MapViewWidget *pWidget, Point3D<uint32> center)
{
    if (m_pLastEmitter != pWidget || !m_buttonHold)
        return;
    switch (m_Type)
    {
    case BRUSH_PEN: drawOnMap(pWidget, center); break;
    }
}

void MapBrush::drawOnMap(MapViewWidget *pWidget, Point3D<uint32> center)
{
    if (!pWidget || !pWidget->getMap())
        return;

    switch (m_Type)
    {
    case BRUSH_PEN:
            _drawPen(pWidget, center);
        break;
    case BRUSH_FILL: _drawFill(pWidget, center); break;
    }

    pWidget->changedMap();
}

void MapBrush::setupBitset(const MAP::MapPrototypePtr &map, BitsetVector &mapBitset)
{
    if (map)
        mapBitset = BitsetVector(map->getSize().x, boost::dynamic_bitset<>(map->getSize().y));
}

void MapBrush::calculateFillArea(MapViewWidget *pWidget, const Point3D<uint32> &center, BitsetVector &mapBitset)
{
    mapBitset.clear();
    MapPrototypePtr map = pWidget->getMap();
    if (!map)
        return;

    MAP::MapTile startTile(map->getMapTile(center));
    if (startTile.m_uiAutoTileSetID == MAX_UINT32 || startTile.m_uiTileID == MAX_UINT32)
        return;

    // new action
    MultiTileMapActionPtr pRedoAction(new MultiTileMapAction(startTile, map, center.z));
    pRedoAction->addPosition(center);

    // store center in open points, and change tile
    setupBitset(map, mapBitset);
    mapBitset.at(center.x)[center.y] = true;
    UInt32PointVector openPoints;
    openPoints.push_back(center);

    UInt32PointVector::const_iterator itr;
    while (!openPoints.empty())
    {
        UInt32PointSet posAround;
        Point<uint32> checkPoint = openPoints.at(0);
        map->getPositionsAroundWithID(getTile(), checkPoint, posAround);
        for (UInt32PointSet::const_iterator itr = posAround.begin(); itr != posAround.end(); ++itr)
        {
            Point3D<uint32> curPos(*itr, center.z);
            // continue if its a diagonal tile
            if (curPos.x != checkPoint.x && curPos.y != checkPoint.y)
                continue;
            if (_checkFill(map, startTile, curPos, mapBitset, openPoints))
                pRedoAction->addPosition(curPos);
        }
        openPoints.erase(openPoints.begin());
    }

    pWidget->addAction(pRedoAction);
}


/*#####
# MapTileBrush
#####*/
void MapTileBrush::_drawFill(MapViewWidget *pWidget, const Point3D<uint32> &center)
{
    if (!pWidget || !pWidget->getMap() || pWidget->getMap()->getTile(center) == getTile())
        return;

    calculateFillArea(pWidget, center);
    pWidget->getMap()->setMapTile(center, MapTile(getTile(), 0));
}

void MapTileBrush::_drawPen(MapViewWidget *pWidget, const Point3D<uint32> &center)
{
    if (!pWidget || !pWidget->getMap())
        return;

    MapPrototypePtr map = pWidget->getMap();
    MapTile mapTile = map->getMapTile(center);
    if (!mapTile.m_uiAutoTileSetID && mapTile.m_uiTileID == getTile())
        return;
    if (mapTile.m_uiTileID != MAX_UINT32)
    {
        pWidget->addAction(MapActionPtr(new TileMapAction(center, mapTile, pWidget->getMap())));
        map->setMapTile(center, MapTile(getTile(), 0));

        // update autotiles around
        UInt32PointSet positions;
        map->getPositionsAroundWithID(0, center, positions, MapPrototype::FLAG_OTHER);
        for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
        {
            Point3D<uint32> pos(*itr, center.z);
            map->setTile(pos, AutoTilePrototype::getAutoTileIndexForTileCheck(map->getPositionsAroundWithID(map->getAutoTile(pos), pos, UInt32PointSet(),
                MapPrototype::FLAG_NOTHING)));
        }
    }
}

bool MapTileBrush::_checkFill(MapPrototypePtr const &map, const MAP::MapTile &centerTile, Point3D<uint32> const &pos,
                          std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints)
{
    // return if already checked
    if (mapBitset.at(pos.x)[pos.y])
        return false;

    mapBitset.at(pos.x)[pos.y] = true;
    MapTile curMapTile = map->getMapTile(pos);
    if (centerTile.m_uiAutoTileSetID == curMapTile.m_uiAutoTileSetID &&
        ((centerTile.m_uiAutoTileSetID == 0 && centerTile.m_uiTileID == curMapTile.m_uiTileID) || centerTile.m_uiAutoTileSetID != 0))
    {
        openPoints.push_back(pos);
        map->setMapTile(pos, MapTile(getTile(), 0));
        return true;
    }
    return false;
}

/*#####
# MapAutoTileBrush
#####*/
void MapAutoTileBrush::_drawPen(MapViewWidget *pWidget, const Point3D<uint32> &center)
{
    m_BorderPosResult.clear();
    ConstAutoTilePrototypePtr proto;
    if (!pWidget || !pWidget->getMap() || !pWidget->getScene() || !m_pAutoTileDB || !m_pAutoTileDB->getPrototype(getTile(), proto) || !proto)
        return;

    uint32 uiOldTile = pWidget->getMap()->getAutoTile(center);
    if (uiOldTile != MAX_UINT32 && getTile() != uiOldTile)
    {
        pWidget->addAction(MapActionPtr(new TileMapAction(center, pWidget->getMap()->getMapTile(center), pWidget->getMap())));
        _setAutoTile(pWidget->getMap(), center, proto);
    }
}

void MapAutoTileBrush::_drawFill(MapViewWidget *pWidget, const Point3D<uint32> &center)
{
    m_BorderPosResult.clear();
    ConstAutoTilePrototypePtr proto;
    MapPrototypePtr map = pWidget->getMap();
    if (!pWidget || !map || !m_pAutoTileDB || !m_pAutoTileDB->getPrototype(getTile(), proto) || !proto || map->getAutoTile(center) == getTile())
        return;

    calculateFillArea(pWidget, center);
    map->setMapTile(center, MapTile(AutoTilePrototype::INDEX_CENTER, getTile()));

    // get border tiles
    UInt32PointSet setBorder;
    for (UInt32PointSet::const_iterator itr = m_BorderPosResult.begin(); itr != m_BorderPosResult.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, center.z);
        if (map->getAutoTile(pos) != getTile())
            map->getPositionsAroundWithID(getTile(), pos, setBorder, MapPrototype::FLAG_SAME);
        else
            map->setTile(pos, AutoTilePrototype::getAutoTileIndexForTileCheck(map->getPositionsAroundWithID(getTile(), pos, setBorder, MapPrototype::FLAG_SAME)));
    }

    // update border tiles
    _doAutoTileCheckForPosList(map, center.z, setBorder);
}

bool MapAutoTileBrush::_checkFill(MapPrototypePtr const &map, const MAP::MapTile &centerTile, Point3D<uint32> const &pos,
                              std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints)
{
    // return if already checked
    if (mapBitset.at(pos.x)[pos.y])
        return false;

    const MapTile curMapTile = map->getMapTile(pos);
    mapBitset.at(pos.x)[pos.y] = true;
    if (centerTile.m_uiAutoTileSetID == curMapTile.m_uiAutoTileSetID)
    {
        // if its not an autotile, the tile id has to be the same
        if (!centerTile.m_uiAutoTileSetID && centerTile.m_uiTileID != curMapTile.m_uiTileID)
            return false;
        openPoints.push_back(pos);
        map->setMapTile(pos, MapTile(AutoTilePrototype::INDEX_CENTER, getTile()));
        return true;
    }
    else
        m_BorderPosResult.insert(pos);
    return false;
}

void MapAutoTileBrush::_setAutoTile(const MAP::MapPrototypePtr &map, const Point3D<uint32> &center, DATABASE::ConstAutoTilePrototypePtr proto)
{
    if (!map || !proto)
        return;

    UInt32PointSet result;
    map->setTile(center, AutoTilePrototype::getAutoTileIndexForTileCheck(map->getPositionsAroundWithID(getTile(), center, result, MapPrototype::FLAG_ALL)));
    map->setAutoTile(center, proto->getID());
    _doAutoTileCheckForPosList(map, center.z, result);
}

void MapAutoTileBrush::_doAutoTileCheckForPosList(const MAP::MapPrototypePtr &map, const uint32 &uiLayer, const UInt32PointSet &positions)
{
    for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
    {
        Point3D<uint32> pos(*itr, uiLayer);
        if (!map->getAutoTile(pos))
            continue;
        map->setTile(pos, AutoTilePrototype::getAutoTileIndexForTileCheck(map->getPositionsAroundWithID(map->getAutoTile(pos), pos, UInt32PointSet(),
            MapPrototype::FLAG_NOTHING)));
    }
}