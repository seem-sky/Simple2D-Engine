#include "MapEditorWidget.h"
#include "Config.h"
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include "moc_MapEditorWidget.h"

using namespace DATABASE;
using namespace MAP;

/*#####
# MapEditor
#####*/
MapEditorWidget::MapEditorWidget(QWidget *pParent) : QWidget(pParent), Ui_MapEditor(), m_pSharedData(new SharedMapEditorData())
{
    setupUi(this);
    m_pWorldObjects->setToolTipPosition(PixmapTooltipTreeWidget::TOOLTIP_RIGHT);
    m_ModifyObj.setWidget(m_pMappingModeTab, MODIFY_RESIZE, QPoint(0, m_pMapTree->height() + 35), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pTileTab, MODIFY_RESIZE, QPoint(0, m_pMapTree->height() + 75), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pObjectTabs, MODIFY_RESIZE, QPoint(0, m_pMapTree->height() + 75), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pMapEditor, MODIFY_RESIZE);
    m_ModifyObj.setWidget(m_pMapTree, MODIFY_MOVE, QPoint(0, 0), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pMapTreeLabel, MODIFY_MOVE, QPoint(0, m_pMapTree->height()), MODIFY_DIRECTION_HEIGHT);

    m_pRevert->setShortcut(QKeySequence(tr("Ctrl+Z", "Map|Revert")));

    connect(m_pTiles, SIGNAL(tileClicked(uint32, Qt::MouseButton)), this, SLOT(_tileClicked(uint32, Qt::MouseButton)));
    connect(m_pAutoTiles, SIGNAL(tileClicked(uint32, Qt::MouseButton)), this, SLOT(_autoTileClicked(uint32, Qt::MouseButton)));

    connect(m_pMapTree, SIGNAL(mapOpened(MAP::MapPrototypePtr)), m_pMapEditor, SLOT(addMapTab(MAP::MapPrototypePtr)));
    connect(m_pMapTree, SIGNAL(mapDeleted(MAP::MapPrototypePtr)), m_pMapEditor, SLOT(closeMap(MAP::MapPrototypePtr)));
    connect(m_pMapTree, SIGNAL(mapUpdated(MAP::MapPrototypePtr)), m_pMapEditor, SLOT(updateMap(MAP::MapPrototypePtr)));

    connect(this, SIGNAL(leftTileChanged(uint32, bool)), m_pLHBrush, SLOT(changeBrush(uint32, bool)));
    connect(this, SIGNAL(rightTileChanged(uint32, bool)), m_pRHBrush, SLOT(changeBrush(uint32, bool)));

    connect(m_pMappingModeTab, SIGNAL(currentChanged(int)), this, SLOT(_MappingModeChanged(int)));
    connect(m_pObjectTabs, SIGNAL(currentChanged(int)), this, SLOT(_objectTabChanged(int)));
    connect(m_pWorldObjects, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(_currentObjectChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(m_pDynamicObjects, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(_currentObjectChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    connect(m_pWorldObjects, SIGNAL(newCursorSet(const QCursor&)), m_pMapEditor, SLOT(setCurrentCursor(const QCursor&)));

    connect(m_pZoom, SIGNAL(valueChanged(int)), this, SLOT(_zoomChanged(int)));
    connect(m_pCurLayer, SIGNAL(valueChanged(int)), this, SLOT(_layerSelected(int)));
    connect(m_pShowGrid, SIGNAL(stateChanged(int)), this, SLOT(_showGridChanged(int)));
    connect(m_pRevert, SIGNAL(clicked(bool)), this, SLOT(_revertButtonClicked(bool)));
    connect(m_pMapEditor, SIGNAL(currentChanged(int)), this, SLOT(_tabSelected(int)));

    _shareData();
    _MappingModeChanged(m_pMappingModeTab->currentIndex());
}

void MapEditorWidget::_tabSelected(int index)
{
    if (MapViewer* pWidget = m_pMapEditor->getCurrentWidget())
    {
        m_pZoom->setValue(pWidget->getZoom());
        m_pCurLayer->setMaximum(pWidget->getMaxLayer(MAP::LAYER_BACKGROUND));
        m_pCurLayer->setValue(pWidget->getCurrentLayer());
        m_pShowGrid->setChecked(pWidget->getShowGrid());
    }
}

void MapEditorWidget::_zoomChanged(int zoom)
{
    if (MapViewer* pWidget = m_pMapEditor->getCurrentWidget())
        pWidget->setZoom(zoom);
}

void MapEditorWidget::_layerSelected(int layer)
{
    if (MapViewer* pWidget = m_pMapEditor->getCurrentWidget())
        pWidget->setCurrentLayer(layer);
}

void MapEditorWidget::_showGridChanged(int state)
{
    if (MapViewer* pWidget = m_pMapEditor->getCurrentWidget())
        pWidget->setShowGrid(m_pShowGrid->isChecked());
}

void MapEditorWidget::_revertButtonClicked(bool clicked)
{
    if (MapViewer* pWidget = m_pMapEditor->getCurrentWidget())
        pWidget->revertAction();
}

void MapEditorWidget::_projectOpened()
{
    updateMapEditorWidgets();
    _loadOpenMapsFromConfig();
}

void MapEditorWidget::_shareData()
{
    m_pMapTree->setSharedData(m_pSharedData);
    m_pMapEditor->setSharedData(m_pSharedData);

    // tile tabs
    m_pTiles->setSharedData(m_pSharedData);
    m_pAutoTiles->setSharedData(m_pSharedData);

    // brushes
    m_pLHBrush->setSharedData(m_pSharedData);
    m_pRHBrush->setSharedData(m_pSharedData);
}

void MapEditorWidget::updateMapEditor()
{
    m_pSharedData->_updateData();
    updateMapEditorWidgets();
}

void MapEditorWidget::updateMapEditorWidgets()
{
    m_pMapTree->updateWidget();
    m_pMapEditor->updateWidget();

    // tile tabs
    m_pTiles->updateWidget();
    m_pAutoTiles->updateWidget();

    // object tabs
    m_pWorldObjects->setAdditionalDBs(m_pSharedData->getSpriteDatabase(), m_pSharedData->getAnimationDatabase());
    m_pWorldObjects->setDB(m_pSharedData->getWorldObjectDatabase());

    // object brushes
    m_MapObjectBrush.setAdditionalDBs(m_pSharedData->getWorldObjectDatabase(), m_pSharedData->getAnimationDatabase(),
                                        m_pSharedData->getSpriteDatabase());

    // brushes
    m_pLHBrush->updateWidget();
    m_pRHBrush->updateWidget();
}

void MapEditorWidget::_loadOpenMapsFromConfig()
{
    const UInt32UnorderedSet openMaps = Config::Get()->getOpenMaps();
    for (UInt32UnorderedSet::const_iterator itr = openMaps.begin(); itr != openMaps.end(); ++itr)
    {
        //MapPrototypePtr map;
        //if (MapDatabasePtr pMapDB = m_pDBMgr->getMapDatabase())
        //{
        //    if (pMapDB->getItem(*itr, map))
        //        m_pMapEditor->addMapTab(map);
        //}
    }
}

void MapEditorWidget::clearWidgets()
{
    for (int i = m_pMapEditor->count()-1; i >= 0; --i)
        m_pMapEditor->closeTab(i);

    // clear tabs
    m_pTiles->clearScene();
    m_pAutoTiles->clearScene();
    m_pWorldObjects->clear();

    m_pMapTree->clear();

    m_pRHBrush->clearWidget();
    m_pLHBrush->clearWidget();
}

void MapEditorWidget::_objectTabChanged(int index)
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

void MapEditorWidget::_currentObjectChanged(QTreeWidgetItem *pItem, QTreeWidgetItem *pPreviousItem)
{
    if (PrototypeTreeWidgetItem *pProtoItem = dynamic_cast<PrototypeTreeWidgetItem*>(pItem))
        m_MapObjectBrush.setObjectID(pProtoItem->text(0).toUInt());
    else
        m_MapObjectBrush.setObjectID(0);
}

void MapEditorWidget::_tileClicked(uint32 uiID, Qt::MouseButton button)
{
    switch (button)
    {
    case Qt::RightButton: emit rightTileChanged(uiID, false); break;
    case Qt::LeftButton: emit leftTileChanged(uiID, false); break;
    }
}

void MapEditorWidget::_autoTileClicked(uint32 uiID, Qt::MouseButton button)
{
    switch (button)
    {
    case Qt::RightButton: emit rightTileChanged(uiID, true); break;
    case Qt::LeftButton: emit leftTileChanged(uiID, true); break;
    }
}

void MapEditorWidget::_MappingModeChanged(int index)
{
    QString sTabText = m_pMappingModeTab->tabText(index);
    if (sTabText == "Tiles")
        _setMappingMode(MODE_TILE);
    else if (sTabText == "Objects")
        _setMappingMode(MODE_OBJECT);

    m_pLayer->setVisible(m_pSharedData->getMappingMode() == MODE_TILE);
    m_pMapEditor->updateWidget();

    // editable objects
    for (int32 i = 0; i < m_pMapEditor->count(); ++i)
    {
        if (MapViewer *pMapView = dynamic_cast<MapViewer*>(m_pMapEditor->widget(i)))
            pMapView->setObjectsEditable(m_pSharedData->getMappingMode() != MODE_TILE);
    }
}

void MapEditorWidget::_pressBrush(MapViewer *pWidget, Point3D<uint32> point, uint32 uiButton)
{
    if (!pWidget || m_pSharedData->getMappingMode() != MODE_TILE)
        return;

    if (Qt::LeftButton == uiButton)
        m_pLHBrush->getMapBrush()->brushPress(pWidget, point);
    else if (Qt::RightButton == uiButton)
        m_pRHBrush->getMapBrush()->brushPress(pWidget, point);
}

void MapEditorWidget::_releaseBrush(MapViewer *pWidget, Point3D<uint32> point, uint32 uiButton)
{
    if (!pWidget)
        return;

    switch (m_pSharedData->getMappingMode())
    {
    case MODE_TILE:
        if (Qt::LeftButton == uiButton)
            m_pLHBrush->getMapBrush()->brushRelease(pWidget, point);
        else if (Qt::RightButton == uiButton)
            m_pRHBrush->getMapBrush()->brushRelease(pWidget, point);
        break;
    case MODE_OBJECT:
        if (Qt::LeftButton == uiButton)
        {
            QPoint pos = pWidget->mapToScene(pWidget->mapFromGlobal(QCursor::pos())).toPoint(); 
            m_MapObjectBrush.drawObject(pWidget, Point3D<uint32>(pos.x(), pos.y()));
        }
        break;
    }
}

void MapEditorWidget::_moveBrush(MapViewer *pWidget, Point3D<uint32> point)
{
    if (!pWidget || m_pSharedData->getMappingMode() != MODE_TILE)
        return;

    m_pLHBrush->getMapBrush()->brushMove(pWidget, point);
    m_pRHBrush->getMapBrush()->brushMove(pWidget, point);
}

void MapEditorWidget::_saveMaps()
{
    for (int i = 0; i < m_pMapEditor->count(); ++i)
    {
        if (MapViewer *pWidget = ((MapViewer*)m_pMapEditor->widget(i)))
        {
            if (pWidget->hasChanged())
                pWidget->saveCurrentMap();
        }
    }
}

void MapEditorWidget::_saveChanges()
{
    m_pSharedData->getMapDatabase()->storeChanges();
    _saveMaps();
}

void MapEditorWidget::setDBMgr(const DATABASE::DatabaseMgrPtr &pDBMgr)
{
    m_pSharedData->_setDatabaseMgr(pDBMgr);
}