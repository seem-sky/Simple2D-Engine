#ifndef MAP_TREE_H
#define MAP_TREE_H

#include <QtWidgets/QTreeWidget>
#include "DatabaseMgr.h"
#include "QtGlobal.h"
#include "MapEditorObject.h"
#include "PrototypeTreeWidgetItem.h"

class MapTreeItem : public PrototypeTreeWidgetItem
{
public:
    MapTreeItem(const DATABASE::MAP_STRUCTURE::MapPrototypePtr &map);

    inline DATABASE::MAP_STRUCTURE::MapPrototypePtr getMap() const { return m_pMap; }
    void updateMapText();

private:
    const DATABASE::MAP_STRUCTURE::MapPrototypePtr m_pMap;
};

class MapTreeWidget : public QTreeWidget, public MapEditorObject
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

    void updateObject();

private slots:
    void _openMapSettingsDialog();
    void _newMap();
    void _deleteMap();
    void _openMap();

signals:
    void mapOpened(DATABASE::MAP_STRUCTURE::MapPrototypePtr pMap);
    void mapDeleted(DATABASE::MAP_STRUCTURE::MapPrototypePtr pMap);
    void mapUpdated(DATABASE::MAP_STRUCTURE::MapPrototypePtr pMap);
};

#endif