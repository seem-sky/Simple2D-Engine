#ifndef MAP_TREE_H
#define MAP_TREE_H

#include <QtGui/QTreeWidget>
#include "Database.h"
#include "MapDatabase.h"

class MapTreeItem : public QTreeWidgetItem
{
public:
    MapTreeItem(const MAP::MapPrototypePtr &map);

    inline MAP::MapPrototypePtr const& getMap() const { return m_pMap; }
    void updateMapText();

    bool operator <(const QTreeWidgetItem &other) const;

private:
    const MAP::MapPrototypePtr m_pMap;
};

class MapTreeWidget : public QTreeWidget
{
    Q_OBJECT
private:
    void _removeItem(MapTreeItem *pItem);
    void _openItemContextMenu(const QPoint &pos);
    void _openWidgetContextMenu(const QPoint &pos);

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent);
    void dropEvent(QDropEvent *pEvent);

public:
    MapTreeWidget(QWidget *pParent = NULL);

private slots:
    void _showMapTree();
    void _openMapSettingsDialog();
    void _newMap();
    void _deleteMap();
    void _openMap();

signals:
    void mapOpened(const MAP::MapPrototypePtr &map);
    void mapUpdated(const MAP::MapPrototypePtr &map);
    void mapDeleted(const MAP::MapPrototypePtr &map);
};

#endif