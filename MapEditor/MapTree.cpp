#include "MapTree.h"
#include "moc_MapTree.h"
#include <QtGui/QMenu>
#include <QtGui/QtEvents>
#include "ContextMenu.h"
#include "MapSettings.h"
#include "Config.h"
#include <QtGui/QMessageBox>

using namespace MAP;

/*#####
# MapTreeItem
#####*/
MapTreeItem::MapTreeItem(const MAP::MapPrototypePtr &map) : m_pMap(map), QTreeWidgetItem()
{
    updateMapText();
}

void MapTreeItem::updateMapText()
{
    if (!m_pMap)
        return;

    setText(0, QString::number(m_pMap->getID()));
    setText(1, QString::fromStdString(m_pMap->getAnnounceName()));
}

bool MapTreeItem::operator <(const QTreeWidgetItem &other) const  
{
    switch (treeWidget()->sortColumn())
    {
    case 0: return text(0).toUInt() < other.text(0).toUInt();
    default: return QTreeWidgetItem::operator <(other);
    }
}

/*#####
# MapTree
#####*/
MapTreeWidget::MapTreeWidget(QWidget *pParent) : QTreeWidget(pParent)
{
    viewport()->installEventFilter(this);
    setIndentation(10);
    connect(window(), SIGNAL(projectLoadDone()), this, SLOT(_showMapTree()));
}

void MapTreeWidget::_openMapSettingsDialog()
{
    if (MapTreeItem *pItem = (MapTreeItem*)currentItem())
    {
        Point3D<uint32> oldSize = pItem->getMap()->getSize();
        MapSettings dialog(pItem->getMap(), this);
        if (dialog.exec())
        {
            pItem->updateMapText();
            emit mapUpdated(pItem->getMap());
        }
    }    
}

void MapTreeWidget::_newMap()
{
    MapDatabase *pMapDB = MapDatabase::Get();
    if (!pMapDB)
        return;

    uint32 uiID = pMapDB->getFreeID();
    MapPrototypePtr newMap(new MapPrototype(uiID, "map" + ToString(uiID) + ".xml"));
    MapSettings dialog(newMap, this);
    if (dialog.exec())
    {
        MapPrototypePtr ptr;
        if (!pMapDB->getMap(newMap->getID(), ptr))
            return;

        addTopLevelItem(new MapTreeItem(ptr));
    }
}

void MapTreeWidget::_deleteMap()
{
    if (MapTreeItem *pItem = (MapTreeItem*)currentItem())
    {
        if (QMessageBox::Ok == QMessageBox::question(0, "delete " + QString::fromStdString(pItem->getMap()->getFileName()),
            QString::fromStdString("Do you really want to delete " + pItem->getMap()->getFileName() + " and all of its children?"), QMessageBox::Ok | QMessageBox::Cancel))
        {
            emit mapDeleted(pItem->getMap());
            _removeItem(pItem);
        }
    } 
}

void MapTreeWidget::_openMap()
{
    if (MapTreeItem *pItem = (MapTreeItem*)currentItem())
    {
        if (MapDatabase *pMapDB = MapDatabase::Get())
            emit mapOpened(pItem->getMap());
    }
}

void MapTreeWidget::_removeItem(MapTreeItem *pItem)
{
    if (!pItem)
        return;

    while (pItem->childCount())
        _removeItem((MapTreeItem*)pItem->child(0));

    if (MapDatabase *pMapDB = MapDatabase::Get())
        pMapDB->removeMap(pItem->getMap()->getID());

    if (pItem->parent())
        pItem->parent()->removeChild(pItem);
    else
        takeTopLevelItem(indexOfTopLevelItem(pItem));

    delete pItem;
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

void MapTreeWidget::_showMapTree()
{
    clear();
    MapDatabase *pMapDB = MapDatabase::Get();
    if (!pMapDB)
        return;

    typedef std::map<uint32/*ID*/, MapTreeItem*> MapItemMap;
    typedef std::vector<MapTreeItem*> MapItemVector;
    MapItemMap mapItems;
    MapItemVector notSortedMaps;
    for (MapPrototypePtrMap::const_iterator itr = pMapDB->getStoredMaps().begin(); itr != pMapDB->getStoredMaps().end(); ++itr)
    {
        MapTreeItem *newItem = new MapTreeItem(itr->second);
        mapItems.insert(std::make_pair(itr->second->getID(), newItem));
        if (itr->second->getParentID() != 0)
        {
            MapItemMap::iterator mapItr = mapItems.find(itr->second->getParentID());
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
    MapTreeItem *pItem = (MapTreeItem*)currentItem();
    QTreeWidget::dropEvent(pEvent);
    if (pItem)
    {
        if (!pItem->parent())
            pItem->getMap()->setParentID(0);
        else
            pItem->getMap()->setParentID(((MapTreeItem*)pItem->parent())->getMap()->getID());
    }
}