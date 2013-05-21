#ifndef MAP_VIEWER_H
#define MAP_VIEWER_H

#include "ModifyView.h"
#include "DatabaseMgr.h"

class MapViewWidget;

/*#####
# MapObjectItem
#####*/
class MapObjectItem : public ModifyItem
{
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *pEvent);

public:
    MapObjectItem(MAP::MapObjectPtr pMapObj) : ModifyItem(), m_pMapObject(pMapObj) {}

    void move(int x, int y);

private:
    MAP::MapObjectPtr m_pMapObject;
};

/*#####
# MapViewScene
#####*/
class MapViewScene : public QGraphicsScene
{
    Q_OBJECT
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent);

public:
    MapViewScene(MapViewWidget *pParent);

    inline void setDBMgr(DATABASE::ConstDatabaseMgrPtr pDBMgr) { m_pDBMgr = pDBMgr; }
    inline DATABASE::ConstDatabaseMgrPtr getDBMgr() const { return m_pDBMgr; }

signals:
    void brushPressed(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void brushReleased(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void brushMoved(MapViewWidget *pWidget, Point3D<uint32> point);

private:
    DATABASE::ConstDatabaseMgrPtr m_pDBMgr;
};

/*#####
# MapView
#####*/
class MapViewer : public ModifyView
{
    Q_OBJECT
public:
    MapViewer(QWidget *pParent);
};
#endif