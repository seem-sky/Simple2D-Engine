#include "MapEditor.h"
#include "Config.h"
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include "moc_MapEditor.h"

using namespace DATABASE;
using namespace MAP;

/*#####
# MapEditor
#####*/
MapEditor::MapEditor(QWidget *pParent) : QWidget(pParent), Ui_MapEditor()
{
    setupUi(this);
    m_pLHBrush = MapBrushPtr(new MapTileBrush());
    m_pRHBrush = MapBrushPtr(new MapTileBrush());
    m_pWorldObjects->setToolTipPosition(PixmapTooltipTreeWidget::TOOLTIP_RIGHT);
    m_pAutoTiles->setAutoTileView();
    m_ModifyObj.setWidget(m_pMappingModeTab, MODIFY_RESIZE, QPoint(0, m_pMapTree->height() + 35), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pTileTab, MODIFY_RESIZE, QPoint(0, m_pMapTree->height() + 75), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pObjectTabs, MODIFY_RESIZE, QPoint(0, m_pMapTree->height() + 75), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pMapEditor, MODIFY_RESIZE);
    m_ModifyObj.setWidget(m_pMapTree, MODIFY_MOVE, QPoint(0, 0), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pMapTreeLabel, MODIFY_MOVE, QPoint(0, m_pMapTree->height()), MODIFY_DIRECTION_HEIGHT);

    connect(m_pMapTree, SIGNAL(mapUpdated(const MAP::MapPrototypePtr&)), m_pMapEditor, SLOT(_updateMap(const MAP::MapPrototypePtr&)));
    connect(m_pMapTree, SIGNAL(mapDeleted(const MAP::MapPrototypePtr&)), m_pMapEditor, SLOT(_closeMap(const MAP::MapPrototypePtr&)));
    connect(m_pMapTree, SIGNAL(mapOpened(const MAP::MapPrototypePtr&)), m_pMapEditor, SLOT(_addMapTab(const MAP::MapPrototypePtr&)));

    connect(m_pTiles, SIGNAL(changeTile(uint32, uint32, bool)), this, SLOT(_currentTileChanged(uint32, uint32, bool)));
    connect(m_pAutoTiles, SIGNAL(changeTile(uint32, uint32, bool)), this, SLOT(_currentTileChanged(uint32, uint32, bool)));
    connect(window(), SIGNAL(projectLoadDone()), this, SLOT(_loadOpenMapsFromConfig()));

    connect(m_pMappingModeTab, SIGNAL(currentChanged(int)), this, SLOT(_MappingModeChanged(int)));
    connect(m_pObjectTabs, SIGNAL(currentChanged(int)), this, SLOT(_objectTabChanged(int)));
    connect(m_pWorldObjects, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(_currentObjectChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(m_pDynamicObjects, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(_currentObjectChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    connect(m_pLHDrawMode, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(_LHDrawModeChanged(const QString&)));
    connect(m_pRHDrawMode, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(_RHDrawModeChanged(const QString&)));

    connect(m_pWorldObjects, SIGNAL(newCursorSet(const QCursor&)), m_pMapEditor, SLOT(setCurrentCursor(const QCursor&)));

    _MappingModeChanged(m_pMappingModeTab->currentIndex());
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

TileBrush::BrushType MapEditor::_getBrushType(const QString &sMode)
{    
    if (sMode == "Fill")
        return TileBrush::BRUSH_FILL;
    return TileBrush::BRUSH_PEN;
}

void MapEditor::_LHDrawModeChanged(const QString &sMode)
{
    m_pLHBrush->setBrushType(_getBrushType(sMode));
}

void MapEditor::_RHDrawModeChanged(const QString &sMode)
{
    m_pRHBrush->setBrushType(_getBrushType(sMode));
}

void MapEditor::_objectTabChanged(int index)
{
    if (m_pObjectTabs->tabText(index) == "WorldObjects")
    {
        m_MapObjectBrush.setObjectType(DATABASE::TYPE_WORLDOBJECT);
        _currentObjectChanged(m_pWorldObjects->currentItem(), NULL);
    }
    else if (m_pObjectTabs->tabText(index) == "DynamicObjects")
    {
        m_MapObjectBrush.setObjectType(DATABASE::TYPE_DYNAMIC_OBJECT);
        //_currentObjectChanged(m_pDynamicObjects->currentItem(), NULL);
    }
}

void MapEditor::_currentObjectChanged(QTreeWidgetItem *pItem, QTreeWidgetItem *pPreviousItem)
{
    if (PrototypeTreeWidgetItem *pProtoItem = dynamic_cast<PrototypeTreeWidgetItem*>(pItem))
        m_MapObjectBrush.setObjectID(pProtoItem->text(0).toUInt());
    else
        m_MapObjectBrush.setObjectID(0);
}

void MapEditor::_updateTiles()
{
    m_TileCache.clear();
    m_AutoTilePixmaps.clear();
    m_pTileTab->fillTiles(m_TileCache);
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
            if (pDB->getPrototype(uiRealTile, proto))
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
                m_pRHBrush = _updateNewPoint(m_pRHBrush, MapBrushPtr(new MapAutoTileBrush(m_pDBMgr->getAutoTileDatabase())));
            else
                m_pRHBrush = _updateNewPoint(m_pRHBrush, MapBrushPtr(new MapTileBrush()));
            m_pRightCurrentTile->setPixmap(*m_TileCache.at(uiRealTile).get());
            m_pRHBrush->setTile(uiSpriteIndex);
            break;
    }
}

void MapEditor::_MappingModeChanged(int index)
{
    QString sTabText = m_pMappingModeTab->tabText(index);
    if (sTabText == "Tiles")
        _setMappingMode(MODE_TILE);
    else if (sTabText == "Objects")
        _setMappingMode(MODE_OBJECT);

    // editable objects
    for (int32 i = 0; i < m_pMapEditor->count(); ++i)
    {
        if (MapViewWidget *pMapView = dynamic_cast<MapViewWidget*>(m_pMapEditor->widget(i)))
            pMapView->setObjectsEditable(m_MappingMode != MODE_TILE);
    }
}

void MapEditor::_pressBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton)
{
    if (!pWidget || m_MappingMode != MODE_TILE)
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

    switch (m_MappingMode)
    {
    case MODE_TILE:
        if (Qt::LeftButton == uiButton)
            m_pLHBrush->brushRelease(pWidget, point);
        else if (Qt::RightButton == uiButton)
            m_pRHBrush->brushRelease(pWidget, point);
        break;
    case MODE_OBJECT:
        if (Qt::LeftButton == uiButton)
        {
            QPoint pos = pWidget->getMapViewer()->mapToScene(pWidget->getMapViewer()->mapFromGlobal(QCursor::pos())).toPoint(); 
            m_MapObjectBrush.drawObject(pWidget, Point3D<uint32>(pos.x(), pos.y()));
        }
        break;
    }
}

void MapEditor::_moveBrush(MapViewWidget *pWidget, Point3D<uint32> point)
{
    if (!pWidget || m_MappingMode != MODE_TILE)
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
    m_pWorldObjects->setDB(WorldObjectDatabaseChangerPtr(new DatabaseChanger<WorldObjectPrototype>(m_pDBMgr->getWorldObjectDatabase())));
    m_pWorldObjects->setAdditionalDBs(m_pDBMgr->getSpriteDatabase(), m_pDBMgr->getAnimationDatabase());
    m_MapObjectBrush.setAdditionalDBs(m_pDBMgr->getWorldObjectDatabase(), m_pDBMgr->getAnimationDatabase(), m_pDBMgr->getSpriteDatabase());
    _updateTiles();
}

void MapEditor::updateMapEditor()
{
    _updateTiles();
    m_pWorldObjects->fillWithPrototypes();
}