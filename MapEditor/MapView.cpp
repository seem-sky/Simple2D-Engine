#include "MapView.h"
#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include "moc_MapView.h"
#include <QtGui/QPainterPath>
#include <QtGui/QMessageBox>
#include <algorithm>
#include "MainWindow.h"
#include "Config.h"
#include "MapIO.h"

using namespace MAP;

/*#####
# MapViewScene
#####*/
MapViewScene::MapViewScene(MapViewWidget *pParent) : QGraphicsScene(pParent)
{
}

void MapViewScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (!parent() || !((MapViewWidget*)parent())->getMap())
        return;

    MapEditor *pEditor = ((MainWindow*)((MapViewWidget*)parent())->window())->getMapEditorWidget();
    if (!pEditor)
        return;
    MapPrototypePtr map = ((MapViewWidget*)parent())->getMap();
    const Point<uint32> startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE);
    const Point<uint32> endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, map->getSize().x),
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, map->getSize().y));
    uint32 uiActiveLayer = ((MapViewWidget*)parent())->getActiveLayer()-1;
    QRectF tileRect(0, 0, TILE_SIZE, TILE_SIZE);
    for (uint32 layer = 0; layer < map->getSize().z; ++layer)
    {
        // draw black rect over lower layer
        if (layer == uiActiveLayer)
            painter->fillRect(rect, QColor(0, 0, 0, 125));
        qreal opacity = layer > uiActiveLayer ? 0.5 : 1;
        UInt32Set usedTileIDs, usedAutoTileIDs;
        std::vector<std::vector<QPainter::PixmapFragment>> tileFragmentVector;
        std::vector<std::map<QPixmapPtr, std::vector<QPainter::PixmapFragment>>> autoTileFragmentVector;
        for (uint32 x = startTile.x; x < endTile.x; ++x)
        {
            for (uint32 y = startTile.y; y < endTile.y; ++y)
            {
                MapTile tileObj = map->getMapTile(Point3D<uint32>(x, y, layer));
                if (tileObj.m_uiTileID == 0 && tileObj.m_uiAutoTileSetID == 0)      // ignore tile ID 0; transparency tile
                    continue;
                // is no auto tile
                if (tileObj.m_uiAutoTileSetID == 0)
                {
                    if (tileObj.m_uiTileID >= tileFragmentVector.size())
                        tileFragmentVector.resize(tileObj.m_uiTileID+1);
                    tileFragmentVector.at(tileObj.m_uiTileID).push_back(QPainter::PixmapFragment::create(QPointF(x*TILE_SIZE+(TILE_SIZE>>1),
                        y*TILE_SIZE+(TILE_SIZE>>1)), tileRect, 1, 1, 0, opacity));
                    usedTileIDs.insert(tileObj.m_uiTileID);
                }
                // autotiles
                else
                {
                    QPixmapPtr pixmapPtr;
                    AutoTilePixmapsPtr pAutoTile;
                    if (tileObj.m_uiAutoTileSetID >= autoTileFragmentVector.size())
                        autoTileFragmentVector.resize(tileObj.m_uiAutoTileSetID+1);

                    if ((pEditor->getAutoTilePixmaps(tileObj.m_uiAutoTileSetID, pAutoTile) || pEditor->createAutoTilePixmaps(tileObj.m_uiAutoTileSetID, pAutoTile)) &&
                        pAutoTile->getPixmap(static_cast<DATABASE::AutoTilePrototype::AUTO_TILE_INDEX>(tileObj.m_uiTileID), pixmapPtr))
                    {
                        std::map<QPixmapPtr, std::vector<QPainter::PixmapFragment>>::iterator itr =
                            autoTileFragmentVector.at(tileObj.m_uiAutoTileSetID).find(pixmapPtr);
                        if (itr == autoTileFragmentVector.at(tileObj.m_uiAutoTileSetID).end())
                        {
                            autoTileFragmentVector.at(tileObj.m_uiAutoTileSetID).insert(std::make_pair(pixmapPtr, std::vector<QPainter::PixmapFragment>()));
                            itr = autoTileFragmentVector.at(tileObj.m_uiAutoTileSetID).find(pixmapPtr);
                        }
                        itr->second.push_back(QPainter::PixmapFragment::create(QPointF(x*TILE_SIZE+(TILE_SIZE>>1), y*TILE_SIZE+(TILE_SIZE>>1)), tileRect, 1, 1, 0, opacity));
                        usedAutoTileIDs.insert(tileObj.m_uiAutoTileSetID);
                    }
                }
            }
        }

        QPixmapPtr pixmapPtr;
        // draw tiles
        for (UInt32Set::const_iterator itr = usedTileIDs.begin(); itr != usedTileIDs.end(); ++itr)
        {
            if (pEditor->getTilePixmap(*itr, pixmapPtr) && pixmapPtr)
                painter->drawPixmapFragments(&tileFragmentVector.at(*itr).at(0), tileFragmentVector.at(*itr).size(), *pixmapPtr.get());
        }
        // draw autoTiles
        for (UInt32Set::const_iterator IDitr = usedAutoTileIDs.begin(); IDitr != usedAutoTileIDs.end(); ++IDitr)
        {
            for (std::map<QPixmapPtr, std::vector<QPainter::PixmapFragment>>::const_iterator itr = autoTileFragmentVector.at(*IDitr).begin();
                itr != autoTileFragmentVector.at(*IDitr).end(); ++itr)
                painter->drawPixmapFragments(&itr->second.at(0), itr->second.size(), *(itr->first.get()));
        }
    }
}

void MapViewScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (!parent() || !((MapViewWidget*)parent())->showGrid() || !((MapViewWidget*)parent())->getMap())
        return;

    MapPrototypePtr map = ((MapViewWidget*)parent())->getMap();
    const Point<uint32> startTile(rect.x() <= 0 ? 0 : (uint32)rect.x() / TILE_SIZE * TILE_SIZE, rect.y() <= 0 ? 0 : (uint32)rect.y() / TILE_SIZE * TILE_SIZE);
    const Point<uint32> endTile(qMin<uint32>(ceil(rect.width() / TILE_SIZE) + startTile.x + 1, map->getSize().x) * TILE_SIZE,
        qMin<uint32>(ceil(rect.height() / TILE_SIZE) + startTile.y + 1, map->getSize().y) * TILE_SIZE);
    QVector<QPoint> pointPairs;
    for (uint32 x = startTile.x; x < endTile.x; x += TILE_SIZE)
    {
        pointPairs.push_back(QPoint(x, startTile.y));
        pointPairs.push_back(QPoint(x, endTile.y));
    }
    for (uint32 y = startTile.y; y < endTile.y; y += TILE_SIZE)
    {
        pointPairs.push_back(QPoint(startTile.x, y));
        pointPairs.push_back(QPoint(endTile.x, y));
    }
    painter->drawLines(pointPairs);
}

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
    m_pView->viewport()->installEventFilter(this);
    m_pView->setScene(new MapViewScene(this));
    _updateMap();
    m_pRevert->setShortcut(QKeySequence(tr("Ctrl+Z", "Map|Revert")));
}

bool MapViewWidget::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (!pObj || !pEvent)
        return false;

    if (pEvent->type() == QEvent::MouseButtonPress && (((QMouseEvent*)pEvent)->button() == Qt::LeftButton || ((QMouseEvent*)pEvent)->button() == Qt::RightButton))
    {
        QPoint point = m_pView->mapToScene(((QMouseEvent*)pEvent)->pos()).toPoint();
        emit brushPressed(this, Point3D<uint32>(point.x() / TILE_SIZE, point.y() / TILE_SIZE, m_pCurLayer->value()-1), ((QMouseEvent*)pEvent)->button());
        return true;
    }
    else if (pEvent->type() == QEvent::MouseButtonRelease && (((QMouseEvent*)pEvent)->button() == Qt::LeftButton || ((QMouseEvent*)pEvent)->button() == Qt::RightButton))
    {
        QPoint point = m_pView->mapToScene(((QMouseEvent*)pEvent)->pos()).toPoint();
        emit brushReleased(this, Point3D<uint32>(point.x() / TILE_SIZE, point.y() / TILE_SIZE, m_pCurLayer->value()-1), ((QMouseEvent*)pEvent)->button());
        return true;
    }
    else if (pEvent->type() == QEvent::MouseMove)
    {
        QPoint point = m_pView->mapToScene(((QMouseEvent*)pEvent)->pos()).toPoint();
        emit brushMoved(this, Point3D<uint32>(point.x() / TILE_SIZE, point.y() / TILE_SIZE, m_pCurLayer->value()-1));
        return true;
    }
    else
        return QWidget::eventFilter(pObj, pEvent);

    return false;
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
        connect(pWidget, SIGNAL(brushPressed(MapViewWidget*, Point3D<uint32>, uint32)), parent(), SLOT(_pressBrush(MapViewWidget*, Point3D<uint32>, uint32)));
        connect(pWidget, SIGNAL(brushReleased(MapViewWidget*, Point3D<uint32>, uint32)), parent(), SLOT(_releaseBrush(MapViewWidget*, Point3D<uint32>, uint32)));
        connect(pWidget, SIGNAL(brushMoved(MapViewWidget*, Point3D<uint32>)), parent(), SLOT(_moveBrush(MapViewWidget*, Point3D<uint32>)));
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
    return getTabWithMap(map) ? true : false;
}