#include "MapEditorWidget.h"
#include "Config.h"
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include "moc_MapEditorWidget.h"
#include <QtWidgets/QApplication>
#include <QtGui/QClipboard>

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

    connect(m_pTileSets, SIGNAL(itemClicked(uint32, Qt::MouseButton)), this, SLOT(_tileSetClicked(uint32, Qt::MouseButton)));
    connect(m_pTiles, SIGNAL(tileClicked(uint32, Qt::MouseButton)), this, SLOT(_tileClicked(uint32, Qt::MouseButton)));
    connect(m_pAutoTiles, SIGNAL(tileClicked(uint32, Qt::MouseButton)), this, SLOT(_autoTileClicked(uint32, Qt::MouseButton)));

    connect(m_pMapTree, SIGNAL(mapOpened(MAP::MapPrototypePtr)), m_pMapEditor, SLOT(addMapTab(MAP::MapPrototypePtr)));
    connect(m_pMapTree, SIGNAL(mapDeleted(MAP::MapPrototypePtr)), m_pMapEditor, SLOT(closeMap(MAP::MapPrototypePtr)));
    connect(m_pMapTree, SIGNAL(mapUpdated(MAP::MapPrototypePtr)), this, SLOT(_mapUpdated(MAP::MapPrototypePtr)));

    connect(this, SIGNAL(leftTileChanged(uint32, MAP::BRUSH::BrushType)), m_pLHBrush, SLOT(changeBrush(uint32, MAP::BRUSH::BrushType)));
    connect(this, SIGNAL(rightTileChanged(uint32, MAP::BRUSH::BrushType)), m_pRHBrush, SLOT(changeBrush(uint32, MAP::BRUSH::BrushType)));

    connect(m_pMappingModeTab, SIGNAL(currentChanged(int)), this, SLOT(_MappingModeChanged(int)));
    connect(m_pObjectTabs, SIGNAL(currentChanged(int)), this, SLOT(_objectTabChanged(int)));
    connect(m_pWorldObjects, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(_currentObjectChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    //connect(m_pDynamicObjects, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(_currentObjectChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    connect(m_pWorldObjects, SIGNAL(newCursorSet(const QCursor&)), m_pMapEditor, SLOT(setCurrentCursor(const QCursor&)));

    // map widget connections
    connect(m_pZoom, SIGNAL(valueChanged(int)), this, SLOT(_zoomChanged(int)));
    connect(m_pShowGrid, SIGNAL(stateChanged(int)), this, SLOT(_showGridChanged(int)));
    connect(m_pRevert, SIGNAL(clicked(bool)), this, SLOT(_revertButtonClicked(bool)));
    connect(m_pMapEditor, SIGNAL(currentChanged(int)), this, SLOT(_tabSelected(int)));

    // tile layer
    connect(m_pLayerForeground, SIGNAL(toggled(bool)), this, SLOT(_layerChanged(bool)));
    connect(m_pLayerBackground, SIGNAL(toggled(bool)), this, SLOT(_layerChanged(bool)));
    _layerChanged(true);

    _shareData();
    _MappingModeChanged(m_pMappingModeTab->currentIndex());
}

void MapEditorWidget::_mapUpdated(MAP::MapPrototypePtr map)
{
    // if current tab, update layer widgets
    if (MapViewer* pTab = m_pMapEditor->getTabWithMap(map))
    {
        pTab->changedMap();
        _layerChanged(true);
    }
}

void MapEditorWidget::_layerChanged(bool state)
{
    if (state)
    {
        disconnect(m_pCurLayer, SIGNAL(valueChanged(int)), this, SLOT(_layerSelected(int)));
        if (MapViewer *pWidget = m_pMapEditor->getCurrentWidget())
        {
            if (m_pLayerBackground->isChecked())
            {
                m_pSharedData->m_CurrentLayer = LAYER_BACKGROUND;
                m_pCurLayer->setMinimum(1);
            }
            else if (m_pLayerForeground->isChecked())
            {
                m_pSharedData->m_CurrentLayer = LAYER_FOREGROUND;
                m_pCurLayer->setMinimum(pWidget->getMaxLayer(LAYER_FOREGROUND) ? 1 : 0);
            }
            pWidget->setLayer(m_pSharedData->getCurrentLayer());
            m_pCurLayer->setMaximum(pWidget->getMaxLayer(m_pSharedData->getCurrentLayer()));
            m_pCurLayer->setValue(pWidget->getCurrentLayer(m_pSharedData->getCurrentLayer()));
            // following line results in an redraw of current map tab
            _layerSelected(m_pCurLayer->value());
            m_pLHBrush->getMapBrush()->setLayer(m_pSharedData->getCurrentLayer());
            m_pRHBrush->getMapBrush()->setLayer(m_pSharedData->getCurrentLayer());
            pWidget->redrawViewport();
        }
        connect(m_pCurLayer, SIGNAL(valueChanged(int)), this, SLOT(_layerSelected(int)));
    }
}

void MapEditorWidget::_tabSelected(int index)
{
    if (MapViewer* pWidget = m_pMapEditor->getCurrentWidget())
    {
        m_pZoom->setValue(pWidget->getZoom());
        disconnect(m_pLayerForeground, SIGNAL(toggled(bool)), this, SLOT(_layerChanged(bool)));
        disconnect(m_pLayerBackground, SIGNAL(toggled(bool)), this, SLOT(_layerChanged(bool)));
        switch (pWidget->getLayer())
        {
        case LAYER_BACKGROUND: m_pLayerBackground->setChecked(true); break;
        case LAYER_FOREGROUND: m_pLayerForeground->setChecked(true); break;
        }
        connect(m_pLayerForeground, SIGNAL(toggled(bool)), this, SLOT(_layerChanged(bool)));
        connect(m_pLayerBackground, SIGNAL(toggled(bool)), this, SLOT(_layerChanged(bool)));
        _layerChanged(true);
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
        pWidget->setCurrentLayer(layer, m_pSharedData->getCurrentLayer());
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

    m_MapObjectBrush.setSharedData(m_pSharedData);
}

void MapEditorWidget::updateMapEditor()
{
    m_pSharedData->_updateData();
    updateMapEditorWidgets();
}

void MapEditorWidget::updateMapEditorWidgets()
{
    m_pMapTree->updateObject();
    m_pMapEditor->updateObject();

    // tile tabs
    m_pTiles->updateObject();
    m_pAutoTiles->updateObject();
    m_pTileSets->setAdditionalDBs(m_pSharedData->getTileDatabase());
    m_pTileSets->setDB(m_pSharedData->getTileSetDatabase());

    // object tabs
    m_pWorldObjects->setAdditionalDBs(m_pSharedData->getSpriteDatabase(), m_pSharedData->getAnimationDatabase());
    m_pWorldObjects->setDB(m_pSharedData->getWorldObjectDatabase());

    // brushes
    m_pLHBrush->updateObject();
    m_pRHBrush->updateObject();
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
    case Qt::RightButton: emit rightTileChanged(uiID, MAP::BRUSH::BRUSH_TILE); break;
    case Qt::LeftButton: emit leftTileChanged(uiID, MAP::BRUSH::BRUSH_TILE); break;
    }
}

void MapEditorWidget::_tileSetClicked(uint32 uiID, Qt::MouseButton button)
{
    switch (button)
    {
    case Qt::RightButton: emit rightTileChanged(uiID, MAP::BRUSH::BRUSH_TILE_SET); break;
    case Qt::LeftButton: emit leftTileChanged(uiID, MAP::BRUSH::BRUSH_TILE_SET); break;
    }
}

void MapEditorWidget::_autoTileClicked(uint32 uiID, Qt::MouseButton button)
{
    switch (button)
    {
    case Qt::RightButton: emit rightTileChanged(uiID, MAP::BRUSH::BRUSH_AUTO_TILE); break;
    case Qt::LeftButton: emit leftTileChanged(uiID, MAP::BRUSH::BRUSH_AUTO_TILE); break;
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
    m_pMapEditor->redrawMaps();

    // editable objects
    for (int32 i = 0; i < m_pMapEditor->count(); ++i)
    {
        if (MapViewer *pMapView = dynamic_cast<MapViewer*>(m_pMapEditor->widget(i)))
            pMapView->setObjectsEditable(m_pSharedData->getMappingMode() != MODE_TILE);
    }
}

void MapEditorWidget::_pressBrush(MapViewer *pWidget, UInt32Point3D point, uint32 uiButton)
{
    if (!pWidget || m_pSharedData->getMappingMode() != MODE_TILE)
        return;

    if (Qt::LeftButton == uiButton)
        m_pLHBrush->getMapBrush()->brushPress(pWidget, point);
    else if (Qt::RightButton == uiButton)
        m_pRHBrush->getMapBrush()->brushPress(pWidget, point);
}

void MapEditorWidget::_releaseBrush(MapViewer *pWidget, UInt32Point3D point, uint32 uiButton)
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
            m_MapObjectBrush.drawObject(pWidget, Int32Point(pos.x(), pos.y()));
        }
        break;
    }
}

void MapEditorWidget::_moveBrush(MapViewer *pWidget, UInt32Point3D point)
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
        if (MapViewer *pWidget = dynamic_cast<MapViewer*>(m_pMapEditor->widget(i)))
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

void MapEditorWidget::storeCurrentMapInClipboard() const
{
    if (MapViewer *pWidget = m_pMapEditor->getCurrentWidget())
    {
        MappingMode mode = m_pSharedData->getMappingMode();
        m_pSharedData->m_MappingMode = MODE_VIEW;
        QRect boundingRect = pWidget->getScene()->itemsBoundingRect().toRect();
        if (boundingRect.x() > 0)
            boundingRect.setX(0);
        if (boundingRect.y() > 0)
            boundingRect.setY(0);
        boundingRect.setTopLeft(pWidget->viewport()->mapFrom(pWidget, pWidget->mapFromScene(boundingRect.topLeft())));
        QSize mapSize(pWidget->getMap()->getSize().x*TILE_SIZE, pWidget->getMap()->getSize().y*TILE_SIZE);
        if (mapSize.width() > boundingRect.width())
            boundingRect.setWidth(mapSize.width());
        if (mapSize.height() > boundingRect.height())
            boundingRect.setHeight(mapSize.height());
        QApplication::clipboard()->setPixmap(pWidget->viewport()->grab(boundingRect));
        m_pSharedData->m_MappingMode = mode;
    }
}