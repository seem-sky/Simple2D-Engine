#ifndef MAP_TREE_H
#define MAP_TREE_H

#include <QtWidgets/QTreeWidget>
#include "MapDatabase.h"
#include <QtWidgets/QMenu>

class MapTreeItem : public QTreeWidgetItem
{
public:
    MapTreeItem(const DATABASE::MAP_STRUCTURE::MapPrototype &map);
    bool operator <(const QTreeWidgetItem &other) const;

    void setup(const DATABASE::MAP_STRUCTURE::MapPrototype &map);
};

class MapEditorModuleMapTree : public QTreeWidget
{
    Q_OBJECT
private:
    void _reload();

protected:
    void dropEvent(QDropEvent *pEvent);

public:
    MapEditorModuleMapTree(QWidget *pParent = nullptr);

    void setDatabase(DATABASE::MAP_STRUCTURE::MapDatabase *pMapDatabase);

private slots:
    void onContextMenuRequested(const QPoint &pos);
    void onActionOpen();
    void onActionEdit();
    void onActionNew();
    void onActionDelete();
    void onProjectSave();

signals:
    void openMap(uint32 mapID);
    void closeMap(uint32 mapID);
    void editMap(uint32 mapID);

private:
    DATABASE::MAP_STRUCTURE::MapDatabase *m_pMapDatabase;
    UInt32List m_DeletedMaps;
};

#endif
