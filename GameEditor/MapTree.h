#ifndef MAP_TREE_H
#define MAP_TREE_H

#include <QtWidgets/QTreeWidget>
#include "MapDatabase.h"
#include <QtWidgets/QMenu>

class MapTree : public QTreeWidget
{
    Q_OBJECT
private:
    void _reload();

public:
    MapTree(QWidget *pParent = nullptr);

    void setDatabase(DATABASE::MAP_STRUCTURE::MapDatabase *pMapDatabase);

private slots:
    void onContextMenuRequested(const QPoint &pos);
    void onActionOpen();
    void onActionEdit();
    void onActionNew();
    void onActionRemove();

private:
    DATABASE::MAP_STRUCTURE::MapDatabase *m_pMapDatabase;
};

#endif
