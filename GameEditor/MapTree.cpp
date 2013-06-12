#include "MapTree.h"
#include "moc_MapTree.h"
#include <QtGui/QMenu>
#include <QtGui/QtEvents>
#include "ContextMenu.h"
#include "MapSettings.h"
#include "Config.h"
#include <QtGui/QMessageBox>
#include "MainWindow.h"

using namespace MAP;
using namespace DATABASE;

/*#####
# MapTreeItem
#####*/
MapTreeItem::MapTreeItem(const MAP::MapPrototypePtr &map) : m_pMap(map), PrototypeTreeWidgetItem(QStringList())
{
    updateMapText();
}

void MapTreeItem::updateMapText()
{
    if (!m_pMap)
        return;

    setText(0, QString::number(m_pMap->getID()));
    setText(1, m_pMap->getName());
}

/*#####
# MapTree
#####*/
MapTreeWidget::MapTreeWidget(QWidget *pParent) : QTreeWidget(pParent)
{
    viewport()->installEventFilter(this);
    setIndentation(10);
}

void MapTreeWidget::_openMapSettingsDialog()
{
    if (MapTreeItem *pItem = dynamic_cast<MapTreeItem*>(currentItem()))
    {
        Point3D<uint32> oldSize = pItem->getMap()->getSize();
        MapSettings dialog(pItem->getMap(), this);
        if (dialog.exec())
        {
            emit mapUpdated(pItem->getMap());
            pItem->updateMapText();
            // add map to changer
            if (m_pSharedData && m_pSharedData->getMapDatabase())
                m_pSharedData->getMapDatabase()->setItem(pItem->getMap()->getID(), pItem->getMap());
        }
    }
}

void MapTreeWidget::_newMap()
{
    if (!m_pSharedData)
        return;

    if (MapDatabaseChangerPtr pMapDB = m_pSharedData->getMapDatabase())
    {
        MapPrototypePtr newMap = pMapDB->getNewMap();
        MapSettings dialog(newMap, this);
        if (dialog.exec())
        {
            pMapDB->setItem(newMap->getID(), newMap);
            addTopLevelItem(new MapTreeItem(newMap));
        }
    }
}

void MapTreeWidget::_deleteMap()
{
    if (MapTreeItem *pItem = dynamic_cast<MapTreeItem*>(currentItem()))
    {
        if (QMessageBox::Ok == QMessageBox::question(0, "delete " + pItem->getMap()->getFileName(),
            "Do you really want to delete " + pItem->getMap()->getFileName() + " and all of its children?", QMessageBox::Ok | QMessageBox::Cancel))
        {
            emit mapDeleted(pItem->getMap());
            _removeItem(pItem);
        }
    } 
}

void MapTreeWidget::_openMap()
{
    if (MapTreeItem *pItem = dynamic_cast<MapTreeItem*>(currentItem()))
    {
        // add map to changer
        if (m_pSharedData && m_pSharedData->getMapDatabase())
            m_pSharedData->getMapDatabase()->setItem(pItem->getMap()->getID(), pItem->getMap());
        emit mapOpened(pItem->getMap());
    }
}

void MapTreeWidget::_removeItem(MapTreeItem *pItem)
{
    if (!pItem || !m_pSharedData)
        return;

    if (MapDatabaseChangerPtr pMapDB = m_pSharedData->getMapDatabase())
    {
        while (pItem->childCount())
            _removeItem((MapTreeItem*)pItem->child(0));

        if (pMapDB)
            pMapDB->removeMap(pItem->getMap()->getID());

        if (pItem->parent())
            pItem->parent()->removeChild(pItem);
        else
            takeTopLevelItem(indexOfTopLevelItem(pItem));

        delete pItem;
    }
}

void MapTreeWidget::_openItemContextMenu(const QPoint &pos)
{
    CONTEXT_MENU::MapItemContextMenu *pMenu = new CONTEXT_MENU::MapItemContextMenu(this);
    pMenu->popup(pos);
}

void MapTreeWidget::_openWidgetContextMenu(const QPoint &pos)
{
    CONTEXT_MENU::MapTreeContextMenu *pMenu = new CONTEXT_MENU::MapTreeContextMenu(this);
    pMenu->popup(pos);
}

void MapTreeWidget::updateWidget()
{
    clear();
    if (!m_pSharedData)
        return;

    if (MapDatabaseChangerPtr pMapDB = m_pSharedData->getMapDatabase())
    {
        typedef std::map<uint32, MapTreeItem*> MapItemMap;
        typedef std::vector<MapTreeItem*> MapItemVector;
        MapItemMap mapItems;
        MapItemVector notSortedMaps;
        MapPrototypePtr pMap;
        for (uint32 i = 1; i < pMapDB->getSize()+1; ++i)
        {
            if (!pMapDB->getItem(i, pMap) || !pMap)
                continue;
            MapTreeItem *newItem = new MapTreeItem(pMap);
            mapItems.insert(std::make_pair(pMap->getID(), newItem));
            if (pMap->getParentID() != 0)
            {
                MapItemMap::iterator mapItr = mapItems.find(pMap->getParentID());
                if (mapItr != mapItems.end())
                    mapItr->second->addChild(newItem);
                else
                    notSortedMaps.push_back(newItem);
            }
            else
                insertTopLevelItem(topLevelItemCount(), newItem);
        }

        while (!notSortedMaps.empty())
        {
            bool erase = false;
            for (MapItemVector::iterator itr = notSortedMaps.begin(); itr < notSortedMaps.end(); ++itr)
            {
                if (erase && itr != notSortedMaps.begin())
                {
                    --itr;
                    erase = false;
                }
                MapItemMap::iterator mapItr = mapItems.find((*itr)->getMap()->getParentID());
                if (mapItr != mapItems.end())
                {
                    mapItr->second->addChild(*itr);
                    notSortedMaps.erase(itr);
                    erase = true;
                    if (notSortedMaps.empty())
                        break;
                }
            }
        }
    }
}

bool MapTreeWidget::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (!pObj || !pEvent)
        return false;

    // open map file
    if (pEvent->type() == QEvent::MouseButtonDblClick && ((QMouseEvent*)pEvent)->button() == Qt::LeftButton)
    {
        _openMap();
        return true;
    }
    // show context menu
    else if (pEvent->type() == QEvent::MouseButtonRelease && ((QMouseEvent*)pEvent)->button() == Qt::RightButton)
    {
        if (currentItem() && currentItem() != headerItem())
            _openItemContextMenu(mapToGlobal(((QMouseEvent*)pEvent)->pos()));
        else
            _openWidgetContextMenu(mapToGlobal(((QMouseEvent*)pEvent)->pos()));

        return true;
    }
    else
        return QTreeWidget::eventFilter(pObj, pEvent);

    return false;
}

void MapTreeWidget::dropEvent(QDropEvent *pEvent)
{
    // update parent ID
    QTreeWidget::dropEvent(pEvent);
    if (MapTreeItem *pItem = dynamic_cast<MapTreeItem*>(currentItem()))
    {
        if (!pItem->parent())
            pItem->getMap()->setParentID(0);
        else
            pItem->getMap()->setParentID(((MapTreeItem*)pItem->parent())->getMap()->getID());
    }
}