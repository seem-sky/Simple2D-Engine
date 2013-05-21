#include "MapView.h"
#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include "moc_MapView.h"
#include <QtGui/QPainterPath>
#include <QtGui/QMessageBox>
#include <algorithm>
#include "Config.h"
#include "MapIO.h"
#include "MainWindow.h"

using namespace MAP;

/*#####
# MapViewWidget
#####*/
MapViewWidget::MapViewWidget(const MapPrototypePtr &map, QWidget* pParent) : m_CurZoom(100), m_pMap(map), QWidget(pParent), Ui_MapViewWidget(),
m_hasChanges(false)
{
    setupUi(this);
    connect(m_pZoom, SIGNAL(valueChanged(int)), this, SLOT(_changeZoom(int)));
    connect(m_pCurLayer, SIGNAL(valueChanged(int)), this, SLOT(_changeCurLayer(int)));
    connect(m_pShowGrid, SIGNAL(stateChanged(int)), this, SLOT(_showGridChanged(int)));
    connect(m_pRevert, SIGNAL(clicked (bool)), this, SLOT(_revertAction(bool)));
    m_ModObj.setWidget(m_pView, MODIFY_RESIZE);
    MapViewScene *pScene = new MapViewScene(this);
    if (MainWindow *pMainWindow = dynamic_cast<MainWindow*>(window()))
        pScene->setDBMgr(pMainWindow->getDatabaseMgr());
    m_pView->setScene(pScene);
    _updateMap();
    m_pRevert->setShortcut(QKeySequence(tr("Ctrl+Z", "Map|Revert")));
}

void MapViewWidget::_changeCurLayer(int layer)
{
    _updateMap();
}

void MapViewWidget::_showGridChanged(int state)
{
    _updateMap();
}

void MapViewWidget::_updateMap()
{
    if (m_pMap)
    {
        m_pCurLayer->setMaximum(m_pMap->getSize().z);
        m_pView->setSceneRect(0, 0, m_pMap->getSize().x*TILE_SIZE, m_pMap->getSize().y*TILE_SIZE);
    }
    m_pView->viewport()->update();
}

void MapViewWidget::_changeZoom(int zoom)
{
    if (!zoom)
        return;

    double newZoom = 100.0/m_CurZoom*zoom/100;
    m_pView->scale(newZoom, newZoom);
    m_CurZoom = zoom;
}

void MapViewWidget::_revertAction(bool checked)
{
    if (m_Actions.empty())
        return;

    m_Actions.at(m_Actions.size()-1)->revertMapAction();
    m_Actions.erase(--m_Actions.end());
    changedMap();
}

void MapViewWidget::addAction(MAP::MapActionPtr pAction)
{
    m_Actions.push_back(pAction);
}

void MapViewWidget::saveCurrentMap()
{
    if (!m_pMap)
        return;

    Config *pConfig = Config::Get();
    if (!pConfig)
        return;

    MapWriter writer(m_pMap);
    bool result = false;
    std::string sFilePath;
    if (MapDatabase::getFilePath(m_pMap, sFilePath, Config::Get()->getProjectDirectory()))
        writer.storeMapThreaded(sFilePath, result);

    m_hasChanges = false;
    updateText();
    m_Actions.clear();
}

void MapViewWidget::updateText()
{
    if (!m_pMap)
        return;

    QString sTabName(QString::fromStdString(m_pMap->getName()));
    if (hasChanged())
        sTabName.append('*');

    emit textUpdated(this, sTabName);
}

void MapViewWidget::changedMap()
{
    m_hasChanges = true;
    _updateMap();
    updateText();
}

void MapViewWidget::setObjectsEditable(bool editable)
{
    MapViewScene *pScene = dynamic_cast<MapViewScene*>(m_pView->scene());
    if (!pScene)
        return;
    QList<QGraphicsItem*> itemList = pScene->items();
    for (QList<QGraphicsItem*>::const_iterator itr = itemList.begin(); itr != itemList.end(); ++itr)
    {
        if (QGraphicsItem *pItem = *itr)
        {
            pItem->setFlag(QGraphicsItem::ItemIsMovable, editable);
            pItem->setFlag(QGraphicsItem::ItemIsSelectable, editable);
            pItem->setFlag(QGraphicsItem::ItemIsFocusable, editable);
            pItem->setSelected(false);
        }
    }
}

/*#####
# MapTabWidget
#####*/
MapTabWidget::MapTabWidget(QWidget* pParent) : QTabWidget(pParent)
{
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(_closeTab(int)));
}

void MapTabWidget::_addMapTab(const MapPrototypePtr &map)
{
    if (!map)
        return;

    MapViewWidget *pWidget = getTabWithMap(map);
    if (!pWidget)
    {
        if (m_pMapDB)
            m_pMapDB->loadMapFile(map->getID(), Config::Get()->getProjectDirectory());
        pWidget = new MapViewWidget(map, this);
        addTab(pWidget, "");
        connect(this, SIGNAL(changedZoom(uint32)), pWidget, SLOT(_changeZoom(uint32)));
        connect(pWidget->getScene(), SIGNAL(brushPressed(MapViewWidget*, Point3D<uint32>, uint32)), parent(), SLOT(_pressBrush(MapViewWidget*, Point3D<uint32>, uint32)));
        connect(pWidget->getScene(), SIGNAL(brushReleased(MapViewWidget*, Point3D<uint32>, uint32)), parent(), SLOT(_releaseBrush(MapViewWidget*, Point3D<uint32>, uint32)));
        connect(pWidget->getScene(), SIGNAL(brushMoved(MapViewWidget*, Point3D<uint32>)), parent(), SLOT(_moveBrush(MapViewWidget*, Point3D<uint32>)));
        connect(pWidget, SIGNAL(textUpdated(MapViewWidget*, const QString&)), this, SLOT(_updateTabText(MapViewWidget*, const QString&)));
        pWidget->updateText();
        Config::Get()->addOpenMap(map->getID());
    }
    setCurrentWidget(pWidget);
}

void MapTabWidget::_updateTabText(MapViewWidget *pWidget, const QString &sTabName)
{
    setTabText(indexOf(pWidget), sTabName);
}

void MapTabWidget::_closeMap(const MapPrototypePtr &map)
{
    if (MapViewWidget* pTab = getTabWithMap(map))
    {
        removeTab(indexOf(pTab));
        Config::Get()->removeOpenMap(map->getID());
    }
}

void MapTabWidget::_updateMap(const MapPrototypePtr &map)
{
    if (MapViewWidget* pTab = getTabWithMap(map))
        pTab->changedMap();
}

void MapTabWidget::_closeTab(int index)
{
    if (MapViewWidget *pTab = (MapViewWidget*)widget(index))
    {
        if (!pTab->getMap())
            return;
        // ask for saving map, if changed
        if (pTab->hasChanged())
        {
            switch (QMessageBox::question(this, "close " + QString::fromStdString(pTab->getMap()->getFileName()), QString::fromStdString(pTab->getMap()->getFileName() +
                " has been changed. Do you want to save changes?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
            {
            case QMessageBox::Yes:
                pTab->saveCurrentMap();
                if (m_pMapDB)
                    m_pMapDB->saveMapInfo(pTab->getMap(), Config::Get()->getProjectDirectory());
                break;
            case QMessageBox::Cancel: return;
            }
        }

        // unload map from memory
        if (m_pMapDB)
            m_pMapDB->unloadMapFile(pTab->getMap()->getID());
        Config::Get()->removeOpenMap(pTab->getMap()->getID());
        removeTab(index);
    }
}

MapViewWidget* MapTabWidget::getTabWithMap(const MAP::MapPrototypePtr &map) const
{
    if (!map)
        return NULL;

    for (int i = 0; i < count(); ++i)
    {
        if (((MapViewWidget*)widget(i))->getMap() == map)
            return (MapViewWidget*)widget(i);
    }
    return NULL;
}


bool MapTabWidget::isMapOpened(const MAP::MapPrototypePtr &map) const
{
    return getTabWithMap(map) != 0;
}

void MapTabWidget::setCurrentCursor(const QCursor &cursor)
{
    m_cursor = cursor;
    // apply cursor to all open map tabs
    for (int32 i = 0; i < count(); ++i)
    {
        if (MapViewWidget *pWidget = dynamic_cast<MapViewWidget*>(widget(i)))
            pWidget->getMapViewer()->setCursor(m_cursor);
    }
}