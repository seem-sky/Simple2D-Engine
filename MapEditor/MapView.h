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

class MapViewWidget;

/*#####
# MapViewScene
#####*/
class MapViewScene : public QGraphicsScene
{
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);

public:
    MapViewScene(MapViewWidget *pParent);
};

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
    bool eventFilter(QObject *pObj, QEvent *pEvent);

public:
    MapViewWidget(const MAP::MapPrototypePtr &map, QWidget *pParent = NULL);

    const MAP::MapPrototypePtr& getMap() const { return m_pMap; }
    inline MapViewScene* getScene() { return (MapViewScene*)m_pView->scene(); }

    void changedMap();
    inline bool hasChanged() const { return m_hasChanges; }

    void drawMap();
    void saveCurrentMap();

    void updateText();
    inline void setMaxLayer(uint32 uiMaxLayer) { m_pCurLayer->setMaximum(uiMaxLayer); }
    inline uint32 getActiveLayer() const { return m_pCurLayer->value(); }

    void addAction(MAP::MapActionPtr pAction);

private slots:
    void _changeZoom(int zoom);
    void _changeCurLayer(int layer);
    void _revertAction(bool);

signals:
    void brushPressed(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void brushReleased(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void brushMoved(MapViewWidget *pWidget, Point3D<uint32> point);
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

public slots:
    void _addMapTab(const MAP::MapPrototypePtr &map);

private slots:
    void _updateMap(const MAP::MapPrototypePtr &map);
    void _closeMap(const MAP::MapPrototypePtr &map);
    void _updateTabText(MapViewWidget *pWidget, QString sTabName);

public slots:
    void _closeTab(int index);
};
#endif