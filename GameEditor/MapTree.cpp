#include "MapTree.h"
#include "moc_MapTree.h"
#include "MapEditorDialogMapSettings.h"

/*#####
# MapTree
#####*/
MapTree::MapTree(QWidget *pParent) : QTreeWidget(pParent), m_pMapDatabase(nullptr)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenuRequested(const QPoint&)));
}

void MapTree::setDatabase(DATABASE::MAP_STRUCTURE::MapDatabase *pMapDatabase)
{
    m_pMapDatabase = pMapDatabase;
    _reload();
}

void MapTree::_reload()
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
        if (pMap->getFileName().isEmpty())
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

void MapTree::onContextMenuRequested(const QPoint &pos)
{
    // setup context menu
    auto pMenu = new QMenu(this);
    if (auto pItem = itemAt(pos))
    {
        setCurrentItem(pItem);
        connect(pMenu->addAction("open"), SIGNAL(triggered()), this, SLOT(onActionOpen()));
        connect(pMenu->addAction("edit"), SIGNAL(triggered()), this, SLOT(onActionEdit()));
        connect(pMenu->addAction("remove"), SIGNAL(triggered()), this, SLOT(onActionRemove()));
    }
    connect(pMenu->addAction("new"), SIGNAL(triggered()), this, SLOT(onActionNew()));
    pMenu->popup(mapToGlobal(pos));
}

void MapTree::onActionEdit()
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

void MapTree::onActionNew()
{
}

void MapTree::onActionOpen()
{
}

void MapTree::onActionRemove()
{
}
