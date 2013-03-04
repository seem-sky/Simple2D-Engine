#include "MapBrush.h"
#include <QtGui/QPainter>
#include <QtGui/QBitmap>
#include "DatabaseOutput.h"
#include "Config.h"

using namespace MAP;

MapBrush::MapBrush()
{
    memset(&m_buttonHold[0], NULL, sizeof(m_buttonHold));
    memset(&m_pLastEmitter[0], NULL, sizeof(m_pLastEmitter));
    memset(&m_uiTileID[0], NULL, sizeof(m_uiTileID));
    memset(&m_Type[0], NULL, sizeof(m_Type));
}

void MapBrush::brushPress(MapViewWidget *pWidget, Point3D<uint32> center, MouseKey uiButton)
{
    m_pLastEmitter[uiButton-1] = pWidget;
    m_buttonHold[uiButton-1] = true;
    drawOnMap(pWidget, center, uiButton);
}

void MapBrush::brushRelease(MapViewWidget *pWidget, Point3D<uint32> center, MouseKey uiButton)
{
    if (m_pLastEmitter[uiButton-1] != pWidget || !m_buttonHold[uiButton-1])
        return;

    m_pLastEmitter[uiButton-1] = pWidget;
    m_buttonHold[uiButton-1] = false;
    switch (m_Type[uiButton-1])
    {
    case BRUSH_RECT: drawOnMap(pWidget, center, uiButton); break;
    }
}

void MapBrush::brushMove(MapViewWidget *pWidget, Point3D<uint32> center)
{
    for (uint32 i = 1; i <= 2; ++i)
    {
        if (m_pLastEmitter[i-1] != pWidget || !m_buttonHold[i-1])
            continue;
        switch (m_Type[i-1])
        {
        case BRUSH_PEN: drawOnMap(pWidget, center, (MouseKey)i); break;
        }
    }
}

void MapBrush::drawOnMap(MapViewWidget *pWidget, Point3D<uint32> center, MouseKey uiButton)
{
    if (!pWidget || !pWidget->getMap() || pWidget->getMap()->getTile(center) == m_uiTileID[uiButton-1])
        return;

    switch (m_Type[uiButton-1])
    {
    case BRUSH_PEN: _drawPen(pWidget, center, uiButton); break;
    case BRUSH_FILL: _drawFill(pWidget, center, uiButton); break;
    }

    pWidget->changedMap();
}

void MapBrush::_drawFill(MapViewWidget *pWidget, const Point3D<uint32> &center, MouseKey uiButton)
{
    if (!pWidget || !pWidget->getMap() || !pWidget->getScene())
        return;

    _calculateFillArea(pWidget, center, uiButton);
}

bool MapBrush::_checkFill(MapPrototypePtr const &map, uint32 const &uiLookUpTileID, uint32 const &uiNewTileID, Point3D<uint32> const &pos,
                               std::vector<boost::dynamic_bitset<>> &mapBitset, UInt32PointVector &openPoints)
{
    if (mapBitset.at(pos.x)[pos.y])
        return false;

    mapBitset.at(pos.x)[pos.y] = true;
    if (uiLookUpTileID == map->getTile(pos))
    {
        openPoints.push_back(pos);
        map->setTile(pos, uiNewTileID);
        return true;
    }
    return false;
}

void MapBrush::_calculateFillArea(MapViewWidget *pWidget, const Point3D<uint32> &center, MouseKey uiButton)
{
    MapPrototypePtr map = pWidget->getMap();
    if (!map)
        return;
    const uint32 uiStartTile(map->getTile(center));
    if (uiStartTile == MAX_UINT32 || m_uiTileID[uiButton-1] == uiStartTile)
        return;

    // new action
    MapActionPtr newAction(new MultiTileMapAction(uiStartTile, map));
    ((MultiTileMapAction*)newAction.get())->addPosition(center);

    // store center in open points, and change tile
    std::vector<boost::dynamic_bitset<>> mapBitset(map->getSize().x, boost::dynamic_bitset<>(map->getSize().y));
    mapBitset.at(center.x)[center.y] = true;
    UInt32PointVector openPoints;
    openPoints.push_back(center);
    map->setTile(center, m_uiTileID[uiButton-1]);

    UInt32PointVector::const_iterator itr;
    while (!openPoints.empty())
    {
        Point<uint32> checkPoint = openPoints.at(0);
        if (checkPoint.y && _checkFill(map, uiStartTile, m_uiTileID[uiButton-1], Point3D<uint32>(checkPoint.x, checkPoint.y-1, center.z), mapBitset, openPoints))
            ((MultiTileMapAction*)newAction.get())->addPosition(Point3D<uint32>(checkPoint.x, checkPoint.y-1, center.z));
        if (checkPoint.x && _checkFill(map, uiStartTile, m_uiTileID[uiButton-1], Point3D<uint32>(checkPoint.x-1, checkPoint.y, center.z), mapBitset, openPoints))
            ((MultiTileMapAction*)newAction.get())->addPosition(Point3D<uint32>(checkPoint.x-1, checkPoint.y, center.z));
        if (checkPoint.y < map->getSize().y-1 && _checkFill(map, uiStartTile, m_uiTileID[uiButton-1], Point3D<uint32>(checkPoint.x, checkPoint.y+1, center.z), mapBitset, openPoints))
            ((MultiTileMapAction*)newAction.get())->addPosition(Point3D<uint32>(checkPoint.x, checkPoint.y+1, center.z));
        if (checkPoint.x < map->getSize().x-1 && _checkFill(map, uiStartTile, m_uiTileID[uiButton-1], Point3D<uint32>(checkPoint.x+1, checkPoint.y, center.z), mapBitset, openPoints))
            ((MultiTileMapAction*)newAction.get())->addPosition(Point3D<uint32>(checkPoint.x+1, checkPoint.y, center.z));
        openPoints.erase(openPoints.begin());
    }

    pWidget->addAction(newAction);
}

void MapBrush::_drawPen(MapViewWidget *pWidget, const Point3D<uint32> &center, MouseKey uiButton)
{
    if (!pWidget || !pWidget->getMap() || !pWidget->getScene())
        return;

    uint32 uiOldTile = pWidget->getMap()->getTile(center);
    if (uiOldTile != MAX_UINT32)
    {
        pWidget->addAction(MapActionPtr(new TileMapAction(center, uiOldTile, pWidget->getMap())));
        pWidget->getMap()->setTile(center, m_uiTileID[uiButton-1]);
    }
}