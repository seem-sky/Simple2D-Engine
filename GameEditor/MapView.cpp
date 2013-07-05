#include "MapView.h"
#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include "moc_MapView.h"
#include <QtGui/QPainterPath>
#include <QtWidgets/QMessageBox>
#include <algorithm>
#include "Config.h"
#include "MapIO.h"

using namespace DATABASE::MAP_STRUCTURE;
using namespace DATABASE;

/*#####
# MapTabWidget
#####*/
MapTabWidget::MapTabWidget(QWidget* pParent) : QTabWidget(pParent), MapEditorObject()
{
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void MapTabWidget::setSharedData(SharedMapEditorDataPtr pData)
{
    MapEditorObject::setSharedData(pData);
    for (int i = 0; i < count(); ++i)
    {
        if (MapViewer* pWidget = dynamic_cast<MapViewer*>(widget(i)))
        {
            if (MapViewScene* pScene = pWidget->getScene())
                pScene->setSharedEditorData(m_pSharedData);
        }
    }
}

void MapTabWidget::addMapTab(MapPrototypePtr map)
{
    if (!map || !m_pSharedData)
        return;

    if (MapDatabaseChangerPtr pMapDB = m_pSharedData->getMapDatabase())
    {
        MapViewer *pWidget = getTabWithMap(map);
        if (!pWidget)
        {
            pMapDB->loadMapFile(map->getID(), Config::Get()->getProjectDirectory());
            pWidget = new MapViewer(map, m_pSharedData, this);
            addTab(pWidget, "");
            connect(pWidget->getScene(), SIGNAL(brushPressed(MapViewer*, UInt32Point3D, uint32)), parent(), SLOT(_pressBrush(MapViewer*, UInt32Point3D, uint32)));
            connect(pWidget->getScene(), SIGNAL(brushReleased(MapViewer*, UInt32Point3D, uint32)), parent(), SLOT(_releaseBrush(MapViewer*, UInt32Point3D, uint32)));
            connect(pWidget->getScene(), SIGNAL(brushMoved(MapViewer*, UInt32Point3D)), parent(), SLOT(_moveBrush(MapViewer*, UInt32Point3D)));
            connect(pWidget, SIGNAL(textUpdated(MapViewer*, const QString&)), this, SLOT(_updateTabText(MapViewer*, const QString&)));
            pWidget->updateText();
            Config::Get()->addOpenMap(map->getID());
        }
        setCurrentWidget(pWidget);
    }
}

void MapTabWidget::_updateTabText(MapViewer *pWidget, const QString &sTabName)
{
    setTabText(indexOf(pWidget), sTabName);
}

void MapTabWidget::closeMap(MapPrototypePtr map)
{
    if (MapViewer* pTab = getTabWithMap(map))
    {
        removeTab(indexOf(pTab));
        Config::Get()->removeOpenMap(map->getID());
    }
}

void MapTabWidget::closeTab(int index)
{
    MapDatabaseChangerPtr pMapDB;
    if (m_pSharedData)
        pMapDB = m_pSharedData->getMapDatabase();

    if (MapViewer *pTab = dynamic_cast<MapViewer*>(widget(index)))
    {
        if (!pTab->getMap())
            return;
        // ask for saving map, if changed
        if (pTab->hasChanged())
        {
            switch (QMessageBox::question(this, "close " + pTab->getMap()->getFileName(), pTab->getMap()->getFileName() +
                " has been changed. Do you want to save changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel))
            {
            case QMessageBox::Yes:
                pTab->saveCurrentMap();
                break;
            case QMessageBox::No:
                pMapDB->undoChanges(pTab->getMap()->getID());
                break;
            case QMessageBox::Cancel: return;
            }
        }

        // unload map from memory
        if (pMapDB)
            pMapDB->unloadMapFile(pTab->getMap()->getID());
        Config::Get()->removeOpenMap(pTab->getMap()->getID());
        removeTab(index);
    }
}

MapViewer* MapTabWidget::getTabWithMap(const MapPrototypePtr &map) const
{
    if (map)
    {
        for (int i = 0; i < count(); ++i)
        {
            if (MapViewer* pWidget = dynamic_cast<MapViewer*>(widget(i)))
            {
                if (pWidget->getMap() == map)
                    return dynamic_cast<MapViewer*>(widget(i));
            }
        }
    }
    return NULL;
}


bool MapTabWidget::isMapOpened(const MapPrototypePtr &map) const
{
    return getTabWithMap(map) != 0;
}

void MapTabWidget::setCurrentCursor(const QCursor &cursor)
{
    m_cursor = cursor;
    // apply cursor to all open map tabs
    //for (int32 i = 0; i < count(); ++i)
    //{
    //    if (MapViewer *pWidget = dynamic_cast<MapViewer*>(widget(i)))
    //        pWidget->viewport()->setCursor(m_cursor);
    //}
}

void MapTabWidget::updateObject()
{
    for (int i = 0; i < count(); ++i)
    {
        if (MapViewer* pWidget = dynamic_cast<MapViewer*>(widget(i)))
            pWidget->updateObject();
    }
}

void MapTabWidget::redrawMaps()
{
    for (int i = 0; i < count(); ++i)
    {
        if (MapViewer* pWidget = dynamic_cast<MapViewer*>(widget(i)))
            pWidget->redrawViewport();
    }
}