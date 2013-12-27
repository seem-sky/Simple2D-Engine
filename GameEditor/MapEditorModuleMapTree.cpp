#include "MapEditorModuleMapTree.h"
#include "moc_MapEditorModuleMapTree.h"
#include "MapEditorDialogMapSettings.h"
#include <QtWidgets/QMessageBox>
#include "DelayedDeleteObject.h"
#include <QtCore/QFile>
#include <QtGui/QDropEvent>
#include "Config.h"

/*#####
# MapTreeItem
#####*/
MapTreeItem::MapTreeItem(const DATABASE::MAP_STRUCTURE::MapPrototype& map) : QTreeWidgetItem()
{
    setup(map);
}

void MapTreeItem::setup(const DATABASE::MAP_STRUCTURE::MapPrototype& map)
{
    setData(0, 0, map.getID());
    setData(1, 0, map.getName());
}

bool MapTreeItem::operator <(const QTreeWidgetItem& other) const
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
MapEditorModuleMapTree::MapEditorModuleMapTree(DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : QTreeWidget(pParent), m_DBMgr(DBMgr)
{
    // header labels
    setColumnCount(0);
    QStringList labels;
    labels.push_back("ID");
    labels.push_back("name");
    setHeaderLabels(labels);

    // other stuff
    setIndentation(15);
    setColumnWidth(0, 50);
    setMaximumSize(200, 200);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenuRequested(const QPoint&)));
}

void MapEditorModuleMapTree::reload()
{
    clear();

    auto pMapDB = m_DBMgr.getMapDatabase();
    uint32 size = pMapDB->getSize();
    std::list<const DATABASE::MAP_STRUCTURE::MapPrototype*> unsortedMaps;
    std::vector<QTreeWidgetItem*> items(size, nullptr);

    for (uint32 i = 1; i <= size; ++i)
    {
        auto pMap = pMapDB->getOriginalPrototype(i);
        if (!pMap->isValid())
            continue;

        // create new QTreeWidgetItem
        auto pItem = new MapTreeItem(*pMap);
        items.at(pMap->getID()-1) = pItem;
        if (!pMap->getParentID())
            addTopLevelItem(pItem);
        else if (auto pParent = items.at(pMap->getParentID()-1))
            pParent->addChild(pItem);
        else
            unsortedMaps.push_back(pMap);
    }

    // insert unsorted items
    const DATABASE::MAP_STRUCTURE::MapPrototype* pFirstUnsortedMap = nullptr;
    while (!unsortedMaps.empty())
    {
        auto pMap =* unsortedMaps.begin();
        unsortedMaps.pop_front();
        if (auto pParent = items.at(pMap->getParentID()-1))
        {
            pParent->addChild(items.at(pMap->getID()-1));
            pFirstUnsortedMap = nullptr;
        }
        else
        {
            unsortedMaps.push_back(pMap);
            // insert maps as topLevelItems
            if (pFirstUnsortedMap == pMap)
            {
                for (auto pInsertMap : unsortedMaps)
                    addTopLevelItem(items.at(pInsertMap->getID()-1));
                break;
            }
            pFirstUnsortedMap = pMap;
        }
    }
}

void MapEditorModuleMapTree::onContextMenuRequested(const QPoint& pos)
{
    // setup context menu
    auto pMenu = new QMenu(this);
    if (auto pItem = itemAt(pos))
    {
        setCurrentItem(pItem);
        connect(pMenu->addAction("open"), SIGNAL(triggered()), this, SLOT(onActionOpen()));
        connect(pMenu->addAction("edit"), SIGNAL(triggered()), this, SLOT(onActionEdit()));
        connect(pMenu->addAction("delete"), SIGNAL(triggered()), this, SLOT(onActionDelete()));
    }
    else
        setCurrentItem(nullptr);
    connect(pMenu->addAction("new"), SIGNAL(triggered()), this, SLOT(onActionNew()));
    pMenu->popup(mapToGlobal(pos));
}

void MapEditorModuleMapTree::onActionEdit()
{
    if (auto pItem = dynamic_cast<MapTreeItem*>(currentItem()))
    {
        if (auto pPrototype = m_DBMgr.getMapDatabase()->getOriginalPrototype(pItem->data(0, 0).toUInt()))
        {
            MapEditorDialogMapSettings dialog(pPrototype, this);
            if (dialog.exec())
            {
                pItem->setup(*pPrototype);
                emit editMap(pPrototype->getID());
            }
        }
    }
}

void MapEditorModuleMapTree::onActionNew()
{
    if (auto pPrototype = m_DBMgr.getMapDatabase()->getNewPrototype())
    {
        pPrototype->setFileName("map" + QString::number(pPrototype->getID()) + ".map");
        MapEditorDialogMapSettings dialog(pPrototype.get(), this);
        if (dialog.exec())
        {
            auto pItem = new MapTreeItem(*pPrototype);
            if (auto pParent = currentItem())
                pParent->addChild(pItem);
            else
                addTopLevelItem(pItem);
            m_DBMgr.getMapDatabase()->setPrototype(pPrototype.release());
        }
    }
}

void MapEditorModuleMapTree::onActionOpen()
{
    if (auto pItem = dynamic_cast<MapTreeItem*>(currentItem()))
        emit openMap(pItem->data(0, 0).toUInt());
}

void MapEditorModuleMapTree::onActionDelete()
{
    if (auto pItem = currentItem())
    {
        if (auto pPrototype = m_DBMgr.getMapDatabase()->getOriginalPrototype(pItem->data(0, 0).toUInt()))
        {
            if (QMessageBox::Yes == QMessageBox::question(this, "Delete map",
                "Do you realy want to delete map\n"
                "ID: " + QString::number(pPrototype->getID()) + "\n" + 
                "name: " + pPrototype->getName() + "?",
                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
            {
                emit closeMap(pPrototype->getID());
                pPrototype->setFileName("");

                // add children to parent item
                auto children = pItem->takeChildren();
                if (auto pParent = pItem->parent())
                {
                    pParent->removeChild(pItem);
                    pParent->addChildren(children);
                }
                else
                {
                    addTopLevelItems(children);
                    takeTopLevelItem(indexOfTopLevelItem(pItem));
                }
                auto pDelayedObject = new DelayedDeleteObject<QTreeWidgetItem>(pItem);
                pDelayedObject->deleteLater();

                m_DeletedMaps.push_back(pPrototype->getID());
            }
        }
    }
}

void MapEditorModuleMapTree::onProjectSave()
{
    // delete maps
    for (auto ID : m_DeletedMaps)
    {
        if (auto pMap = m_DBMgr.getMapDatabase()->getOriginalPrototype(ID))
        {
            QFile::remove(Config::get()->getProjectDirectory()+"/maps/"+pMap->getFileName());
            pMap->setFileName("");
        }
    }
    m_DeletedMaps.clear();
}

void MapEditorModuleMapTree::dropEvent(QDropEvent* pEvent)
{
    auto items = selectedItems();
    MapTreeItem* pItem = nullptr;
    if (!items.isEmpty())
        pItem = dynamic_cast<MapTreeItem*>(items.first());
    
    QTreeWidget::dropEvent(pEvent);

    // update parent ID
    if (pItem)
    {
        if (auto pMap = m_DBMgr.getMapDatabase()->getOriginalPrototype(pItem->data(0, 0).toUInt()))
        {
            uint32 parentID = 0;
            if (pItem->parent())
                parentID = pItem->parent()->data(0, 0).toUInt();
            pMap->setParentID(parentID);
        }
    }
}
