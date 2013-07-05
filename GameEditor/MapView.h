#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QGraphicsView>
#include "MapDatabase.h"
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsPathItem>
#include "DatabaseMgr.h"
#include "MapEditorObject.h"
#include "ModifyObject.h"
#include "MapViewer.h"

/*#####
# MapTabWidget
#####*/
class MapTabWidget : public QTabWidget, public MapEditorObject
{
    Q_OBJECT
public:
    MapTabWidget(QWidget *pParent = NULL);

    bool isMapOpened(const DATABASE::MAP_STRUCTURE::MapPrototypePtr &map) const;
    MapViewer* getTabWithMap(const DATABASE::MAP_STRUCTURE::MapPrototypePtr &map) const;
    MapViewer* getCurrentWidget() const { return dynamic_cast<MapViewer*>(currentWidget()); }

    void updateObject();
    void setSharedData(SharedMapEditorDataPtr pData);

    void redrawMaps();

public slots:
    void addMapTab(DATABASE::MAP_STRUCTURE::MapPrototypePtr map);
    void closeMap(DATABASE::MAP_STRUCTURE::MapPrototypePtr map);

    void closeTab(int index);
    void setCurrentCursor(const QCursor &cursor);

private slots:
    void _updateTabText(MapViewer *pWidget, const QString &sTabName);
    
private:
    QCursor m_cursor;
};
#endif