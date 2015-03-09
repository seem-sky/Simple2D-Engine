#ifndef MAP_TREE_H
#define MAP_TREE_H

#include <QtWidgets/QTreeWidget>
#include <Global.h>
#include <Database/Derived.h>

class MapTreeItem : public QTreeWidgetItem
{
public:
    MapTreeItem(const database::prototype::Map& map);
    bool operator <(const QTreeWidgetItem& other) const;

    void setup(const database::prototype::Map& map);
};

class MapEditorModuleMapTree : public QTreeWidget
{
    Q_OBJECT
protected:
    void dropEvent(QDropEvent* pEvent);

public:
    MapEditorModuleMapTree(database::MapDatabase& mapDB, QWidget* pParent = nullptr);

    void reload();
    void save(const QString& path);

private slots:
    void onContextMenuRequested(const QPoint& pos);
    void onActionOpen();
    void onActionEdit();
    void onActionNew();
    void onActionDelete();

signals:
    void openMap(uint32 mapID);
    void closeMap(uint32 mapID);
    void editMap(uint32 mapID);

private:
    database::MapDatabase& m_MapDB;
    std::list<std::unique_ptr<database::prototype::Map>> m_DeletedMaps;
};
#endif
