#ifndef MAP_TREE_H
#define MAP_TREE_H

#include <QtWidgets/QTreeWidget>
#include "DatabaseMgr.h"
#include <QtWidgets/QMenu>

class MapTreeItem : public QTreeWidgetItem
{
public:
    MapTreeItem(const DATABASE::MAP_STRUCTURE::MapPrototype& map);
    bool operator <(const QTreeWidgetItem& other) const;

    void setup(const DATABASE::MAP_STRUCTURE::MapPrototype& map);
};

class MapEditorModuleMapTree : public QTreeWidget
{
    Q_OBJECT
protected:
    void dropEvent(QDropEvent* pEvent);

public:
    MapEditorModuleMapTree(DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);

    void reload();

private slots:
    void onContextMenuRequested(const QPoint& pos);
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
    DATABASE::DatabaseMgr& m_DBMgr;
    UInt32List m_DeletedMaps;
};

#endif
