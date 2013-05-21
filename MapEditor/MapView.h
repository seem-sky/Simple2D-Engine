#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include <QtGui/QTabWidget>
#include <QtGui/QGraphicsView>
#include "MapDatabase.h"
#include "ui/UI_MapViewWidget.h"
#include "ModifyObject.h"
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QGraphicsPathItem>
#include <boost/multi_array.hpp>
#include "MapAction.h"
#include "DatabaseMgr.h"

/*#####
# MapViewWidget
#####*/
class MapViewWidget : public QWidget, Ui_MapViewWidget
{
    Q_OBJECT
private:
    void _updateMap();

protected:
    void resizeEvent(QResizeEvent *pEvent) { m_ModObj.resizeEvent(this); }

public:
    MapViewWidget(const MAP::MapPrototypePtr &map, QWidget *pParent = NULL);

    const MAP::MapPrototypePtr& getMap() const { return m_pMap; }
    inline MapViewScene* getScene() { return dynamic_cast<MapViewScene*>(m_pView->scene()); }
    inline MapViewer* getMapViewer() { return m_pView; }
    inline uint32 getCurrentLayer() const { return m_pCurLayer->value(); }

    void changedMap();
    inline bool hasChanged() const { return m_hasChanges; }

    void drawMap();
    void saveCurrentMap();

    void updateText();
    inline void setMaxLayer(uint32 uiMaxLayer) { m_pCurLayer->setMaximum(uiMaxLayer); }
    inline uint32 getActiveLayer() const { return m_pCurLayer->value(); }

    void addAction(MAP::MapActionPtr pAction);

    inline bool showGrid() const { return m_pShowGrid->isChecked(); }

    void setObjectsEditable(bool editable = true);

private slots:
    void _changeZoom(int zoom);
    void _changeCurLayer(int layer);
    void _revertAction(bool);
    void _showGridChanged(int state);

signals:
    void textUpdated(MapViewWidget *pWidget, const QString &sTabName);

private:
    MAP::MapPrototypePtr m_pMap;
    int m_CurZoom;
    ModifyObject m_ModObj;
    bool m_hasChanges;
    MAP::MapActionPtrVector m_Actions;
};

/*#####
# MapTabWidget
#####*/
class MapTabWidget : public QTabWidget
{
    Q_OBJECT
private:
    void _updateZoom();

public:
    MapTabWidget(QWidget *pParent = NULL);

    bool isMapOpened(const MAP::MapPrototypePtr &map) const;
    MapViewWidget* getTabWithMap(const MAP::MapPrototypePtr &map) const;
    inline void setMapDatabase(DATABASE::MapDatabasePtr pMapDB) { m_pMapDB = pMapDB; }

public slots:
    void _addMapTab(const MAP::MapPrototypePtr &map);
    void _closeTab(int index);
    void setCurrentCursor(const QCursor &cursor);

private slots:
    void _updateMap(const MAP::MapPrototypePtr &map);
    void _closeMap(const MAP::MapPrototypePtr &map);
    void _updateTabText(MapViewWidget *pWidget, const QString &sTabName);
    
private:
    DATABASE::MapDatabasePtr m_pMapDB;

    QCursor m_cursor;
};
#endif