#include "MapEditor.h"
#include "Config.h"
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include "moc_MapEditor.h"

using namespace DATABASE;
using namespace MAP;

/*#####
# AutoTilePixmaps
#####*/
void AutoTilePixmaps::_createPixmaps()
{
    _clearPixmaps();
    if (!m_pProto || !m_pEditor)
        return;

    // setup the easy ones
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        QPixmapPtr pNewPixmap;
        if (m_pEditor->getTilePixmap(m_pProto->getTileID(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i)), pNewPixmap))
            _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // inner edges
    for (uint32 i = AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT; i <= AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_RIGHT; ++i)
    {
        QPoint pos(0, 0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT:
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_LEFT:
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_RIGHT:
            pos.setX(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pInnerCenter;
        QPixmapPtr pCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_CENTER), pCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pCenter.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // multi inner edges
    for (uint32 i = AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT; i <= AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT; ++i)
    {
        QPoint pos1(0, 0);
        QPoint pos2(0, 0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_RIGHT:
            pos2.setX(TILE_SIZE/2);
            pos2.setY(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT:
            pos1.setX(TILE_SIZE/2);
            pos2.setY(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT:
            pos2.setX(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT:
            pos2.setY(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_RIGHT:
            pos1.setX(TILE_SIZE/2);
            pos2.setX(TILE_SIZE/2);
            pos2.setY(TILE_SIZE/2);
            break;

        case AutoTilePrototype::INDEX_INNER_EDGE_BOTTOM_LEFT_BOTTOM_RIGHT:
            pos1.setY(TILE_SIZE/2);
            pos2.setX(TILE_SIZE/2);
            pos2.setY(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pInnerCenter;
        QPixmapPtr pCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_CENTER), pCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pCenter.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos1.x(), pos1.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos1.x(), pos1.y());
        pixmapPainter.drawTiledPixmap(pos2.x(), pos2.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos2.x(), pos2.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // double sides
    for (uint32 i = AutoTilePrototype::INDEX_SIDE_VERTICAL; i <= AutoTilePrototype::INDEX_SIDE_HORIZONTAL; ++i)
    {
        QSize size(TILE_SIZE, TILE_SIZE);
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex[2];
        switch (i)
        {
        case AutoTilePrototype::INDEX_SIDE_VERTICAL:
            uiIndex[0] = AutoTilePrototype::INDEX_RIGHT;
            uiIndex[1] = AutoTilePrototype::INDEX_LEFT;
            size.setWidth(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_SIDE_HORIZONTAL:
            uiIndex[0] = AutoTilePrototype::INDEX_BOTTOM;
            uiIndex[1] = AutoTilePrototype::INDEX_TOP;
            size.setHeight(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pSide[2];
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex[0]), pSide[0]) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex[1]), pSide[1]))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pSide[0].get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(0, 0, size.width(), size.height(), *pSide[1].get(), 0, 0);
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // T tiles
    for (uint32 i = AutoTilePrototype::INDEX_T_TOP; i <= AutoTilePrototype::INDEX_T_RIGHT; ++i)
    {
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex;
        QPoint pos(0,0);
        QSize size(TILE_SIZE, TILE_SIZE);
        switch (i)
        {
        case AutoTilePrototype::INDEX_T_TOP:
            uiIndex = AutoTilePrototype::INDEX_TOP;
            size.setHeight(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_T_BOTTOM:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM;
            size.setHeight(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_T_LEFT:
            uiIndex = AutoTilePrototype::INDEX_LEFT;
            size.setWidth(TILE_SIZE/2);
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_T_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_RIGHT;
            size.setWidth(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pSide;
        QPixmapPtr pInnerCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex), pSide) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pSide.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), size.width(), size.height(), *pInnerCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // Y tiles
    for (uint32 i = AutoTilePrototype::INDEX_Y_TOP_BOTTOM_LEFT; i <= AutoTilePrototype::INDEX_Y_RIGHT_BOTTOM_LEFT; ++i)
    {
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex;
        QPoint pos(0,0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_Y_TOP_BOTTOM_LEFT:
            uiIndex = AutoTilePrototype::INDEX_TOP;
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_TOP_BOTTOM_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_TOP;
            pos.setY(TILE_SIZE/2);
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_BOTTOM_TOP_LEFT:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM;
            break;
        case AutoTilePrototype::INDEX_Y_BOTTOM_TOP_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM;
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_LEFT_TOP_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_LEFT;
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_LEFT_BOTTOM_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_LEFT;
            pos.setX(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_Y_RIGHT_TOP_LEFT:
            uiIndex = AutoTilePrototype::INDEX_RIGHT;
            break;
        case AutoTilePrototype::INDEX_Y_RIGHT_BOTTOM_LEFT:
            uiIndex = AutoTilePrototype::INDEX_RIGHT;
            pos.setY(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pSide;
        QPixmapPtr pInnerCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex), pSide) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pSide.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // curves
    for (uint32 i = AutoTilePrototype::INDEX_CURVE_TOP_LEFT; i <= AutoTilePrototype::INDEX_CURVE_BOTTOM_RIGHT; ++i)
    {
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex;
        QPoint pos(0,0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_CURVE_TOP_LEFT:
            uiIndex = AutoTilePrototype::INDEX_TOP_LEFT;
            pos.setX(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_CURVE_TOP_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_TOP_RIGHT;
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_CURVE_BOTTOM_LEFT:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM_LEFT;
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_CURVE_BOTTOM_RIGHT:
            uiIndex = AutoTilePrototype::INDEX_BOTTOM_RIGHT;
            break;
        }
        QPixmapPtr pEdge;
        QPixmapPtr pInnerCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex), pEdge) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pEdge.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), TILE_SIZE/2, TILE_SIZE/2, *pInnerCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // triple inner edges
    for (uint32 i = AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT; i <= AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT; ++i)
    {
        QPoint pos(0,0);
        switch (i)
        {
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT:
            pos.setX(TILE_SIZE/2);
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT:
            pos.setY(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT:
            pos.setX(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_INNER_EDGE_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT:
            break;
        }
        QPixmapPtr pCenter;
        QPixmapPtr pInnerCenter;
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_CENTER), pCenter) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_INNER_CENTER), pInnerCenter))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pInnerCenter.get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(pos.x(), pos.y(), TILE_SIZE/2, TILE_SIZE/2, *pCenter.get(), pos.x(), pos.y());
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // side ends
    for (uint32 i = AutoTilePrototype::INDEX_SIDE_END_TOP; i <= AutoTilePrototype::INDEX_SIDE_END_RIGHT; ++i)
    {
        QSize size(TILE_SIZE, TILE_SIZE);
        AutoTilePrototype::AUTO_TILE_INDEX uiIndex[2];
        switch (i)
        {
        case AutoTilePrototype::INDEX_SIDE_END_TOP:
            uiIndex[0] = AutoTilePrototype::INDEX_TOP_RIGHT;
            uiIndex[1] = AutoTilePrototype::INDEX_TOP_LEFT;
            size.setWidth(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_SIDE_END_BOTTOM:
            uiIndex[0] = AutoTilePrototype::INDEX_BOTTOM_RIGHT;
            uiIndex[1] = AutoTilePrototype::INDEX_BOTTOM_LEFT;
            size.setWidth(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_SIDE_END_LEFT:
            uiIndex[0] = AutoTilePrototype::INDEX_BOTTOM_LEFT;
            uiIndex[1] = AutoTilePrototype::INDEX_TOP_LEFT;
            size.setHeight(TILE_SIZE/2);
            break;
        case AutoTilePrototype::INDEX_SIDE_END_RIGHT:
            uiIndex[0] = AutoTilePrototype::INDEX_BOTTOM_RIGHT;
            uiIndex[1] = AutoTilePrototype::INDEX_TOP_RIGHT;
            size.setHeight(TILE_SIZE/2);
            break;
        }
        QPixmapPtr pSide[2];
        if (!m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex[0]), pSide[0]) ||
            !m_pEditor->getTilePixmap(m_pProto->getTileID(uiIndex[1]), pSide[1]))
            break;
        QPixmapPtr pNewPixmap(new QPixmap(*pSide[0].get()));
        QPainter pixmapPainter(pNewPixmap.get());
        pixmapPainter.drawTiledPixmap(0, 0, size.width(), size.height(), *pSide[1].get(), 0, 0);
        _setPixmap(static_cast<AutoTilePrototype::AUTO_TILE_INDEX>(i), pNewPixmap);
    }

    // circle
    QPixmapPtr pSide[4];
    if (!m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_TOP_LEFT), pSide[0]) ||
        !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_TOP_RIGHT), pSide[1]) ||
        !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_BOTTOM_LEFT), pSide[2]) ||
        !m_pEditor->getTilePixmap(m_pProto->getTileID(AutoTilePrototype::INDEX_BOTTOM_RIGHT), pSide[3]))
        return;
    QPixmapPtr pNewPixmap(new QPixmap(*pSide[0].get()));
    QPainter pixmapPainter(pNewPixmap.get());
    pixmapPainter.drawTiledPixmap(TILE_SIZE/2, 0, TILE_SIZE/2, TILE_SIZE/2, *pSide[1].get(), TILE_SIZE/2, 0);
    pixmapPainter.drawTiledPixmap(0, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, *pSide[2].get(), 0, TILE_SIZE/2);
    pixmapPainter.drawTiledPixmap(TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, TILE_SIZE/2, *pSide[3].get(), TILE_SIZE/2, TILE_SIZE/2);
    _setPixmap(AutoTilePrototype::INDEX_INNER_EDGE_CENTER, pNewPixmap);
}


/*#####
# MapEditor
#####*/
MapEditor::MapEditor(QWidget *pParent) : QWidget(pParent), Ui_MapEditor()
{
    setupUi(this);
    m_pLHBrush = MapBrushPtr(new MapTileBrush());
    m_pRHBrush = MapBrushPtr(new MapTileBrush());
    m_pAutoTiles->setAutoTileView();
    m_ModifyObj.setWidget(m_pMappingModeTab, MODIFY_RESIZE, QPoint(0, m_pMapTree->height() + 35), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pTileTab, MODIFY_RESIZE, QPoint(0, m_pMapTree->height() + 193), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pMapEditor, MODIFY_RESIZE);
    m_ModifyObj.setWidget(m_pMapTree, MODIFY_MOVE, QPoint(0, 0), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pMapTreeLabel, MODIFY_MOVE, QPoint(0, m_pMapTree->height()), MODIFY_DIRECTION_HEIGHT);

    connect(m_pMapTree, SIGNAL(mapUpdated(const MAP::MapPrototypePtr&)), m_pMapEditor, SLOT(_updateMap(const MAP::MapPrototypePtr&)));
    connect(m_pMapTree, SIGNAL(mapDeleted(const MAP::MapPrototypePtr&)), m_pMapEditor, SLOT(_closeMap(const MAP::MapPrototypePtr&)));
    connect(m_pMapTree, SIGNAL(mapOpened(const MAP::MapPrototypePtr&)), m_pMapEditor, SLOT(_addMapTab(const MAP::MapPrototypePtr&)));

    connect(m_pTiles, SIGNAL(changeTile(uint32, uint32, bool)), this, SLOT(_currentTileChanged(uint32, uint32, bool)));
    connect(m_pAutoTiles, SIGNAL(changeTile(uint32, uint32, bool)), this, SLOT(_currentTileChanged(uint32, uint32, bool)));
    connect(window(), SIGNAL(projectLoadDone()), this, SLOT(_loadOpenMapsFromConfig()));

    connect(m_pLeftBrushPen, SIGNAL(toggled(bool)), this, SLOT(_setLeftPen(bool)));
    connect(m_pLeftBrushRect, SIGNAL(toggled(bool)), this, SLOT(_setLeftRect(bool)));
    connect(m_pLeftBrushFill, SIGNAL(toggled(bool)), this, SLOT(_setLeftFill(bool)));
    connect(m_pRightBrushPen, SIGNAL(toggled(bool)), this, SLOT(_setRightPen(bool)));
    connect(m_pRightBrushRect, SIGNAL(toggled(bool)), this, SLOT(_setRightRect(bool)));
    connect(m_pRightBrushFill, SIGNAL(toggled(bool)), this, SLOT(_setRightFill(bool)));
}

void MapEditor::_loadOpenMapsFromConfig()
{
    const UInt32UnorderedSet openMaps = Config::Get()->getOpenMaps();
    for (UInt32UnorderedSet::const_iterator itr = openMaps.begin(); itr != openMaps.end(); ++itr)
    {
        MapPrototypePtr map;
        if (MapDatabasePtr pMapDB = m_pDBMgr->getMapDatabase())
        {
            if (pMapDB->getPrototype(*itr, map))
                m_pMapEditor->_addMapTab(map);
        }
    }
}

void MapEditor::clearWidgets()
{
    for (int i = m_pMapEditor->count()-1; i >= 0; --i)
        m_pMapEditor->_closeTab(i);

    m_pTileTab->clearTiles();
    m_pMapTree->clear();
    m_pLeftCurrentTile->clear();
    m_pRightCurrentTile->clear();
}

bool MapEditor::getTilePixmap(uint32 uiTileID, QPixmapPtr &result) const
{
    if (uiTileID < m_TileCache.size())
    {
        result = m_TileCache.at(uiTileID);
        return true;
    }
    return false;
}

void MapEditor::_setLeftPen(bool checked)
{
    if (checked)
        m_pLHBrush->setBrushType(MapBrush::BRUSH_PEN);
}

void MapEditor::_setLeftRect(bool checked)
{
    if (checked)
        m_pLHBrush->setBrushType(MapBrush::BRUSH_RECT);
}

void MapEditor::_setLeftFill(bool checked)
{
    if (checked)
        m_pLHBrush->setBrushType(MapBrush::BRUSH_FILL);
}

void MapEditor::_setRightPen(bool checked)
{
    if (checked)
        m_pRHBrush->setBrushType(MapBrush::BRUSH_PEN);
}

void MapEditor::_setRightRect(bool checked)
{
    if (checked)
        m_pRHBrush->setBrushType(MapBrush::BRUSH_RECT);
}

void MapEditor::_setRightFill(bool checked)
{
    if (checked)
        m_pRHBrush->setBrushType(MapBrush::BRUSH_FILL);
}

void MapEditor::_updateTiles()
{
    updateTiles();
}

bool MapEditor::createAutoTilePixmaps(uint32 uiID, AutoTilePixmapsPtr &result)
{
    if (!m_pDBMgr || !uiID)
        return false;
    ConstAutoTileDatabasePtr pDB = m_pDBMgr->getAutoTileDatabase();
    ConstAutoTilePrototypePtr proto;
    if (!pDB || !pDB->getPrototype(uiID, proto))
        return false;
    if (--uiID >= m_AutoTilePixmaps.size())
        m_AutoTilePixmaps.resize(uiID+1);
    AutoTilePixmapsPtr pPixmap(new AutoTilePixmaps());
    pPixmap->setMapEditorWidget(this);
    pPixmap->setPrototype(proto);
    m_AutoTilePixmaps.at(uiID) = pPixmap;
    result = pPixmap;
    return true;
}

bool MapEditor::getAutoTilePixmaps(uint32 uiID, AutoTilePixmapsPtr &result) const
{
    if (!uiID || --uiID >= m_AutoTilePixmaps.size() || !m_AutoTilePixmaps.at(uiID))
        return false;
    result = m_AutoTilePixmaps.at(uiID);
    return true;
}

MAP::MapBrushPtr& MapEditor::_updateNewPoint(const MAP::MapBrushPtr &pOldBrush, MAP::MapBrushPtr &pNewBrush)
{
    pNewBrush->setBrushType(pOldBrush->getBrushType());
    pNewBrush->setTile(pOldBrush->getTile());
    return pNewBrush;
}

void MapEditor::_currentTileChanged(uint32 uiSpriteIndex, uint32 uiButton, bool autoTile)
{
    uint32 uiRealTile = uiSpriteIndex;
    // if autotile, change shown sprite
    if (autoTile)
    {
        if (ConstAutoTileDatabasePtr pDB = m_pDBMgr->getAutoTileDatabase())
        {
            ConstAutoTilePrototypePtr proto; 
            if (pDB->getPrototype(uiRealTile, proto) && proto)
                uiRealTile = proto->getTileID(AutoTilePrototype::INDEX_CENTER);
        }
    }

    if (uiRealTile >= m_TileCache.size() || !m_TileCache.at(uiRealTile))
        return;

    switch (uiButton)
    {
        case Qt::LeftButton:
            if (autoTile)
                m_pLHBrush = _updateNewPoint(m_pLHBrush, MapBrushPtr(new MapAutoTileBrush(m_pDBMgr->getAutoTileDatabase())));
            else
                m_pLHBrush = _updateNewPoint(m_pLHBrush, MapBrushPtr(new MapTileBrush()));
            m_pLeftCurrentTile->setPixmap(*m_TileCache.at(uiRealTile).get());
            m_pLHBrush->setTile(uiSpriteIndex);
            break;
        case Qt::RightButton:
            if (autoTile)
                m_pLHBrush = _updateNewPoint(m_pRHBrush, MapBrushPtr(new MapAutoTileBrush(m_pDBMgr->getAutoTileDatabase())));
            else
                m_pLHBrush = _updateNewPoint(m_pRHBrush, MapBrushPtr(new MapTileBrush()));
            m_pRightCurrentTile->setPixmap(*m_TileCache.at(uiRealTile).get());
            m_pRHBrush->setTile(uiSpriteIndex);
            break;
    }
}

void MapEditor::_pressBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton)
{
    if (!pWidget)
        return;

    if (Qt::LeftButton == uiButton)
        m_pLHBrush->brushPress(pWidget, point);
    else if (Qt::RightButton == uiButton)
        m_pRHBrush->brushPress(pWidget, point);
}

void MapEditor::_releaseBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton)
{
    if (!pWidget)
        return;

    if (Qt::LeftButton == uiButton)
        m_pLHBrush->brushRelease(pWidget, point);
    else if (Qt::RightButton == uiButton)
        m_pRHBrush->brushRelease(pWidget, point);
}

void MapEditor::_moveBrush(MapViewWidget *pWidget, Point3D<uint32> point)
{
    if (!pWidget)
        return;

    m_pLHBrush->brushMove(pWidget, point);
    m_pRHBrush->brushMove(pWidget, point);
}

void MapEditor::_saveMaps()
{
    for (int i = 0; i < m_pMapEditor->count(); ++i)
    {
        if (MapViewWidget *pWidget = ((MapViewWidget*)m_pMapEditor->widget(i)))
        {
            if (pWidget->hasChanged())
                pWidget->saveCurrentMap();
        }
    }
}

void MapEditor::setDBMgr(const DATABASE::DatabaseMgrPtr &pDBMgr)
{
    m_pDBMgr = pDBMgr;
    m_pMapEditor->setMapDatabase(m_pDBMgr->getMapDatabase());
    m_pMapTree->setMapDB(m_pDBMgr->getMapDatabase());
    m_pTileTab->setTileDB(m_pDBMgr->getTileDatabase());
    m_pTileTab->setAutoTileDB(m_pDBMgr->getAutoTileDatabase());
    updateTiles();
}

void MapEditor::updateTiles()
{
    m_TileCache.clear();
    m_AutoTilePixmaps.clear();
    m_pTileTab->fillTiles(m_TileCache);
}