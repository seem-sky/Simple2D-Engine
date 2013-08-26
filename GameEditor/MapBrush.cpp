#include "MapBrush.h"
#include <QtGui/QPainter>
#include <QtGui/QBitmap>
#include "Config.h"
#include "AnimationViewWidget.h"
#include "ObjectAnimationWidget.h"

using namespace DATABASE;
using namespace DATABASE::MAP_STRUCTURE;
using namespace BRUSH;

BrushMode BRUSH::getBrushMode(const QString &mode)
{
    if (mode == "Fill")
        return MODE_FILL;
    return MODE_PEN;
}

/*#####
# MapBrush
#####*/
TileBrush::TileBrush() : m_buttonHold(false), m_pLastEmitter(NULL), m_mode(MODE_PEN), m_uiTileID(0), m_Layer(MAP::LAYER_BACKGROUND)
{}

void TileBrush::brushPress(MapViewer *pWidget, UInt32Point3D center)
{
    m_pLastEmitter = pWidget;
    m_buttonHold = true;
    drawOnMap(pWidget, center);
}

void TileBrush::brushRelease(MapViewer *pWidget, UInt32Point3D center)
{
    if (m_pLastEmitter != pWidget || !m_buttonHold)
        return;

    m_pLastEmitter = pWidget;
    m_buttonHold = false;
    switch (m_mode)
    {
    case MODE_RECT: drawOnMap(pWidget, center); break;
    }
}

void TileBrush::brushMove(MapViewer *pWidget, UInt32Point3D center)
{
    if (m_pLastEmitter != pWidget || !m_buttonHold)
        return;
    switch (m_mode)
    {
    case MODE_PEN: drawOnMap(pWidget, center); break;
    }
}

void TileBrush::drawOnMap(MapViewer *pWidget, UInt32Point3D center)
{
    if (!pWidget || !pWidget->getMap())
        return;

    switch (m_mode)
    {
    case MODE_PEN:
            _drawPen(pWidget, center);
        break;
    case MODE_FILL: _drawFill(pWidget, center); break;
    }

    pWidget->changedMap();
}

void TileBrush::setupBitset(const MapPrototypePtr &map, BitsetVector &mapBitset)
{
    if (map)
        mapBitset = BitsetVector(map->getSize().x, boost::dynamic_bitset<>(map->getSize().y));
}

void TileBrush::calculateFillArea(MapViewer *pWidget, const UInt32Point3D &center, BitsetVector &mapBitset)
{
    mapBitset.clear();
    MapPrototypePtr map = pWidget->getMap();
    if (!map)
        return;

    MAP::MapTile startTile(map->getMapTile(center, m_Layer));
    if (startTile.m_uiAutoTileSetID == MAX_UINT32 || startTile.m_uiTileID == MAX_UINT32)
        return;

    // new action
    MultiPositionTileMapActionPtr pRedoAction(new MultiPositionTileMapAction(startTile, map, center.z, m_Layer));
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
        UInt32Point checkPoint = openPoints.at(0);
        map->checkAutoTiles(getTile(), checkPoint, posAround, m_Layer);
        for (UInt32PointSet::const_iterator itr = posAround.begin(); itr != posAround.end(); ++itr)
        {
            UInt32Point3D curPos(*itr, center.z);
            // continue if its a diagonal tile
            if (curPos.x != checkPoint.x && curPos.y != checkPoint.y)
                continue;
            if (_checkFill(map, startTile, curPos, mapBitset, openPoints))
                pRedoAction->addPosition(curPos);
        }
        openPoints.erase(openPoints.begin());
    }

    pWidget->addAction(std::move(pRedoAction));
}

/*#####
# MapTileBrush
#####*/
void MapTileBrush::_drawFill(MapViewer *pWidget, const UInt32Point3D &center)
{
    if (!pWidget || !pWidget->getMap() || pWidget->getMap()->getTile(center, getLayer()) == getTile())
        return;

    calculateFillArea(pWidget, center);
    pWidget->getMap()->setMapTile(center, MapTile(getTile(), 0), getLayer());
}

void MapTileBrush::_drawPen(MapViewer *pWidget, const UInt32Point3D &center)
{
    if (!pWidget || !pWidget->getMap())
        return;

    MapPrototypePtr map = pWidget->getMap();
    MapTile mapTile = map->getMapTile(center, getLayer());
    if (!mapTile.m_uiAutoTileSetID && mapTile.m_uiTileID == getTile())
        return;
    if (mapTile.m_uiTileID != MAX_UINT32)
    {
        pWidget->addAction(MapActionPtr(new TileMapAction(center, mapTile, getLayer(), pWidget->getMap())));
        map->setMapTile(center, MapTile(getTile(), 0), getLayer());

        if (mapTile.m_uiAutoTileSetID)
        {
            // update autotiles around
            UInt32PointSet positions;
            map->checkAutoTiles(0, center, positions, getLayer(), MapPrototype::FLAG_OTHER);
            for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
            {
                UInt32Point3D pos(*itr, center.z);
                map->setTile(pos, AUTO_TILE::getAutoTileIndexForTileCheck(map->checkAutoTiles(map->getAutoTile(pos, getLayer()), pos, UInt32PointSet(),
                    getLayer(), MapPrototype::FLAG_NOTHING)),getLayer());
            }
        }
    }
}

bool MapTileBrush::_checkFill(MapPrototypePtr map, MapTile centerTile, UInt32Point3D pos, BitsetVector &mapBitset, UInt32PointVector &openPoints)
{
    // return if already checked
    if (mapBitset.at(pos.x)[pos.y])
        return false;

    mapBitset.at(pos.x)[pos.y] = true;
    MapTile curMapTile = map->getMapTile(pos, getLayer());
    if (centerTile.m_uiAutoTileSetID == curMapTile.m_uiAutoTileSetID &&
        ((centerTile.m_uiAutoTileSetID == 0 && centerTile.m_uiTileID == curMapTile.m_uiTileID) || centerTile.m_uiAutoTileSetID != 0))
    {
        openPoints.push_back(pos);
        map->setMapTile(pos, MapTile(getTile(), 0), getLayer());
        return true;
    }
    return false;
}

/*#####
# MapTileSetBrush
#####*/
void MapTileSetBrush::_drawFill(MapViewer *pWidget, const UInt32Point3D &center)
{
    if (!pWidget || !pWidget->getMap())
        return;

    m_centerPos = center;
    calculateFillArea(pWidget, center);
    ConstTileSetPrototypePtr tileSetProto;
    if (m_pTileSetDB->getItem(getTile(), tileSetProto))
        pWidget->getMap()->setMapTile(center, MapTile(tileSetProto->getTileID(UInt32Point(0, 0)), 0), getLayer());
    m_centerPos = UInt32Point3D();
}

void MapTileSetBrush::_drawPen(MapViewer *pWidget, const UInt32Point3D &center)
{
    if (!pWidget || !pWidget->getMap() || !m_pTileSetDB)
        return;

    MapPrototypePtr map = pWidget->getMap();
    ConstTileSetPrototypePtr proto;
    if (m_pTileSetDB->getItem(getTile(), proto))
    {
        MultiPositionMultiTileMapActionPtr pRedoAction(new MultiPositionMultiTileMapAction(map, center.z, getLayer()));
        bool changed = false;
        for (uint32 x = 0; x < proto->getTileCount().x; ++x)
        {
            for (uint32 y = 0; y < proto->getTileCount().y; ++y)
            {
                if (uint32 uiNewID = proto->getTileID(UInt32Point(x, y)))
                {
                    UInt32Point3D currentPos(center.x + x, center.y + y, center.z);
                    MapTile oldTile = map->getMapTile(currentPos, getLayer());
                    if (oldTile.m_uiAutoTileSetID != 0 || oldTile.m_uiTileID != uiNewID)
                    {
                        changed = true;
                        map->setMapTile(currentPos, MapTile(uiNewID, 0), getLayer());
                        pRedoAction->addPosition(currentPos, oldTile);
                        // update autotiles around
                        if (oldTile.m_uiAutoTileSetID)
                        {
                            UInt32PointSet positions;
                            map->checkAutoTiles(0, currentPos, positions, getLayer(), MapPrototype::FLAG_OTHER);
                            for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
                            {
                                UInt32Point3D pos(*itr, center.z);
                                map->setTile(pos, AUTO_TILE::getAutoTileIndexForTileCheck(map->checkAutoTiles(map->getAutoTile(pos, getLayer()), pos, UInt32PointSet(),
                                    getLayer(), MapPrototype::FLAG_NOTHING)),getLayer());
                            }
                        }
                    }
                }
            }
        }
        if (changed)
            pWidget->addAction(std::move(pRedoAction));
    }
}

bool MapTileSetBrush::_checkFill(MapPrototypePtr map, MapTile centerTile, UInt32Point3D pos, BitsetVector &mapBitset, UInt32PointVector &openPoints)
{
    // return if already checked
    if (mapBitset.at(pos.x)[pos.y] || !m_pTileSetDB)
        return false;

    mapBitset.at(pos.x)[pos.y] = true;
    MapTile curMapTile = map->getMapTile(pos, getLayer());
    if (centerTile.m_uiAutoTileSetID == curMapTile.m_uiAutoTileSetID &&
        ((centerTile.m_uiAutoTileSetID == 0 && centerTile.m_uiTileID == curMapTile.m_uiTileID) || centerTile.m_uiAutoTileSetID != 0))
    {
        openPoints.push_back(pos);
        ConstTileSetPrototypePtr tileSetProto;
        if (m_pTileSetDB->getItem(getTile(), tileSetProto))
        {
            
            UInt32Point index(pos.x % tileSetProto->getTileCount().x - m_centerPos.x % tileSetProto->getTileCount().x,
                pos.y % tileSetProto->getTileCount().y - m_centerPos.y % tileSetProto->getTileCount().y);
            // if index overflows, add tileCount
            if (index.x > tileSetProto->getTileCount().x)
                index.x += tileSetProto->getTileCount().x;
            if (index.y > tileSetProto->getTileCount().y)
                index.y += tileSetProto->getTileCount().y;
            map->setMapTile(pos, MapTile(tileSetProto->getTileID(index), 0), getLayer());
        }
        return true;
    }
    return false;
}

/*#####
# MapAutoTileBrush
#####*/
void MapAutoTileBrush::_drawPen(MapViewer *pWidget, const UInt32Point3D &center)
{
    m_BorderPosResult.clear();
    ConstAutoTilePrototypePtr proto;
    if (!pWidget || !pWidget->getMap() || !pWidget->getScene() || !m_pAutoTileDB || !m_pAutoTileDB->getItem(getTile(), proto) || !proto)
        return;

    uint32 uiOldTile = pWidget->getMap()->getAutoTile(center, getLayer());
    if (uiOldTile != MAX_UINT32 && getTile() != uiOldTile)
    {
        pWidget->addAction(MapActionPtr(new TileMapAction(center, pWidget->getMap()->getMapTile(center, getLayer()), getLayer(), pWidget->getMap())));
        _setAutoTile(pWidget->getMap(), center, proto);
    }
}

void MapAutoTileBrush::_drawFill(MapViewer *pWidget, const UInt32Point3D &center)
{
    m_BorderPosResult.clear();
    ConstAutoTilePrototypePtr proto;
    MapPrototypePtr map = pWidget->getMap();
    if (!pWidget || !map || !m_pAutoTileDB || !m_pAutoTileDB->getItem(getTile(), proto) || !proto || map->getAutoTile(center, getLayer()) == getTile())
        return;

    calculateFillArea(pWidget, center);
    map->setMapTile(center, MapTile(AUTO_TILE::INDEX_CENTER, getTile()), getLayer());

    // get border tiles
    UInt32PointSet setBorder;
    for (UInt32PointSet::const_iterator itr = m_BorderPosResult.begin(); itr != m_BorderPosResult.end(); ++itr)
    {
        UInt32Point3D pos(*itr, center.z);
        if (map->getAutoTile(pos, getLayer()) != getTile())
            map->checkAutoTiles(getTile(), pos, setBorder, getLayer(), MapPrototype::FLAG_SAME);
        else
            map->setTile(pos, AUTO_TILE::getAutoTileIndexForTileCheck(map->checkAutoTiles(getTile(), pos, setBorder, getLayer(), MapPrototype::FLAG_SAME)), getLayer());
    }

    // update border tiles
    _doAutoTileCheckForPosList(map, center.z, setBorder);
}

bool MapAutoTileBrush::_checkFill(MapPrototypePtr map, MapTile centerTile, UInt32Point3D pos, BitsetVector &mapBitset, UInt32PointVector &openPoints)
{
    // return if already checked
    if (mapBitset.at(pos.x)[pos.y])
        return false;

    const MapTile curMapTile = map->getMapTile(pos, getLayer());
    mapBitset.at(pos.x)[pos.y] = true;
    if (centerTile.m_uiAutoTileSetID == curMapTile.m_uiAutoTileSetID)
    {
        // if its not an autotile, the tile id has to be the same
        if (!centerTile.m_uiAutoTileSetID && centerTile.m_uiTileID != curMapTile.m_uiTileID)
            return false;
        openPoints.push_back(pos);
        map->setMapTile(pos, MapTile(AUTO_TILE::INDEX_CENTER, getTile()), getLayer());
        return true;
    }
    else
        m_BorderPosResult.insert(pos);
    return false;
}

void MapAutoTileBrush::_setAutoTile(const MapPrototypePtr &map, const UInt32Point3D &center, DATABASE::ConstAutoTilePrototypePtr proto)
{
    if (!map || !proto)
        return;

    UInt32PointSet result;
    map->setTile(center, AUTO_TILE::getAutoTileIndexForTileCheck(map->checkAutoTiles(getTile(), center, result, getLayer(), MapPrototype::FLAG_ALL)), getLayer());
    map->setAutoTile(center, proto->getID(), getLayer());
    _doAutoTileCheckForPosList(map, center.z, result);
}

void MapAutoTileBrush::_doAutoTileCheckForPosList(const MapPrototypePtr &map, const uint32 &uiLayer, const UInt32PointSet &positions)
{
    for (UInt32PointSet::const_iterator itr = positions.begin(); itr != positions.end(); ++itr)
    {
        UInt32Point3D pos(*itr, uiLayer);
        if (!map->getAutoTile(pos, getLayer()))
            continue;
        map->setTile(pos, AUTO_TILE::getAutoTileIndexForTileCheck(map->checkAutoTiles(map->getAutoTile(pos, getLayer()), pos, UInt32PointSet(),
            getLayer(), MapPrototype::FLAG_NOTHING)), getLayer());
    }
}

/*#####
# MapObjectBrush
#####*/
bool MapObjectBrush::drawObject(MapViewer *pWidget, Int32Point pos)
{
    if (!pWidget || !pWidget->getMap() || !pWidget->getScene() || !m_pSharedData)
        return false;

    MapViewScene *pScene = dynamic_cast<MapViewScene*>(pWidget->getScene());

    // add pixmap to MapObjectItem
    QRect boundingRect;
    MapObjectItem *pItem = new MapObjectItem(pWidget->getMap()->addMapObject(m_ObjectType, m_uiObjectID, pos));
    pItem->setPixmap(getObjectPixmap(m_uiObjectID, m_ObjectType, DIRECTION_DOWN, m_pSharedData->getWorldObjectDatabase(), m_pSharedData->getAnimationDatabase(),
                    m_pSharedData->getSpriteDatabase(), boundingRect));
    pScene->addItem(pItem);
    pItem->move(boundingRect.x() + pos.x, boundingRect.y() + pos.y);
    return true;
}

QPixmap MapObjectBrush::getObjectPixmap(uint32 uiObjectID, DATABASE::MAP_OBJECT::ObjectType type, MapDirection direction, DATABASE::ConstWorldObjectDatabasePtr pWorldObjectDB,
                                        DATABASE::ConstAnimationDatabasePtr pAnimationDB, DATABASE::ConstSpriteDatabasePtr pSpriteDB, QRect &boundingRect)
{
    if (!uiObjectID || !pWorldObjectDB || !pAnimationDB || !pSpriteDB)
        return QPixmap();

    // get worldobject
    ConstWorldObjectPrototypePtr objectProto;
    if (!pWorldObjectDB->getItem(uiObjectID, objectProto))
        return false;
    // create frame
    MAP_OBJECT::AnimationInfo animationInfo = objectProto->getAnimationInfo(direction);
    ConstAnimationPrototypePtr pAnimation;
    if (!pAnimationDB->getItem(animationInfo.m_uiAnimationID, pAnimation))
        QPixmap();
    ObjectAnimationWidget viewer(pAnimation, pSpriteDB, 0);
    boundingRect = viewer.scene()->itemsBoundingRect().toRect();
    viewer.resize(boundingRect.width(), boundingRect.height());
    //AnimationViewDB aniViewer;
    //aniViewer.setAttribute(Qt::WA_TranslucentBackground);
    //aniViewer.setWindowFlags(Qt::FramelessWindowHint);
    //aniViewer.setStyleSheet("background:transparent");
    //aniViewer.setFrameShape(QFrame::NoFrame);
    //aniViewer.setAnimationDB(pAnimationDB);
    //aniViewer.setSpriteDB(pSpriteDB);
    //aniViewer.setGridDraw(false);
    //aniViewer.setPreviousFrameDraw(false);
    //aniViewer.setCurrentAnimation(animationInfo.m_uiAnimationID);
    //// resize viewer
    //AnimationViewScene *pAniScene = dynamic_cast<AnimationViewScene*>(aniViewer.scene());
    //if (!pAniScene)
    //    return false;
    //boundingRect = pAniScene->itemsBoundingRect().toRect();
    //aniViewer.resize(boundingRect.width(), boundingRect.height());
    //aniViewer.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //aniViewer.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    return viewer.grab();
}