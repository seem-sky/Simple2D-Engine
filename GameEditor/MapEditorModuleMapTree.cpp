#include "MapEditorModuleMapTree.h"
#include "moc_MapEditorModuleMapTree.h"
#include "MapEditorDialogMapSettings.h"
#include <QtWidgets/QMessageBox>
#include "DelayedDeleteObject.h"
#include <QtCore/QFile>
#include "Config.h"

/*#####
# MapTree
#####*/
MapEditorModuleMapTree::MapEditorModuleMapTree(QWidget *pParent) : QTreeWidget(pParent), m_pMapDatabase(nullptr)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenuRequested(const QPoint&)));
}

void MapEditorModuleMapTree::setDatabase(DATABASE::MAP_STRUCTURE::MapDatabase *pMapDatabase)
{
    m_pMapDatabase = pMapDatabase;
    _reload();
}

void MapEditorModuleMapTree::_reload()
{
    clear();
    if (!m_pMapDatabase)
        return;

    uint32 size = m_pMapDatabase->getSize();
    std::list<const DATABASE::MAP_STRUCTURE::MapPrototype*> unsortedMaps;
    std::vector<QTreeWidgetItem*> items(size, nullptr);

    for (uint32 i = 1; i <= size; ++i)
    {
        auto pMap = m_pMapDatabase->getOriginalPrototype(i);
        if (!pMap->isValid())
            continue;

        // create new QTreeWidgetItem
        QStringList strings;
        strings.push_back(QString::number(pMap->getID()));
        strings.push_back(pMap->getName());
        auto pItem = new QTreeWidgetItem(strings);
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
        auto pMap = *unsortedMaps.begin();
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

void MapEditorModuleMapTree::onContextMenuRequested(const QPoint &pos)
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
    if (auto pItem = currentItem())
    {
        if (auto pPrototype = m_pMapDatabase->getOriginalPrototype(pItem->data(0, 0).toUInt()))
        {
            MapEditorDialogMapSettings dialog(pPrototype, this);
            dialog.exec();
        }
    }
}

void MapEditorModuleMapTree::onActionNew()
{
    if (auto pPrototype = m_pMapDatabase->getNewPrototype())
    {
        pPrototype->setFileName("map" + QString::number(pPrototype->getID()) + ".map");
        MapEditorDialogMapSettings dialog(pPrototype.get(), this);
        if (dialog.exec())
        {
            QStringList strings;
            strings.push_back(QString::number(pPrototype->getID()));
            strings.push_back(pPrototype->getName());
            auto pItem = new QTreeWidgetItem(strings);
            if (auto pParent = currentItem())
                pParent->addChild(pItem);
            else
                addTopLevelItem(pItem);
            m_pMapDatabase->setPrototype(pPrototype.release());
        }
    }
}

void MapEditorModuleMapTree::onActionOpen()
{
}

void MapEditorModuleMapTree::onActionDelete()
{
    if (auto pItem = currentItem())
    {
        if (auto pPrototype = m_pMapDatabase->getOriginalPrototype(pItem->data(0, 0).toUInt()))
        {
            if (QMessageBox::Yes == QMessageBox::question(this, "Delete map",
                "Do you realy want to delete map\n"
                "ID: " + QString::number(pPrototype->getID()) + "\n" + 
                "name: " + pPrototype->getName() + "?",
                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
            {
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
        if (auto pMap = m_pMapDatabase->getOriginalPrototype(ID))
        {
            QFile::remove(Config::get()->getProjectDirectory()+"/maps/"+pMap->getFileName());
            pMap->setFileName("");
        }
    }
    m_DeletedMaps.clear();
}
