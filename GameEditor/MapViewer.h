#ifndef MAP_VIEWER_H
#define MAP_VIEWER_H

#include "ModifyView.h"
#include "DatabaseMgr.h"
#include "MapEditorObject.h"
#include "MapAction.h"

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
class MapViewer;
class MapViewScene : public QGraphicsScene
{
    Q_OBJECT
private:
    void _drawGrid(QPainter *painter, const QRectF &rect);
    void _drawUpperLayer(QPainter *painter, const QRectF &rect);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent);

public:
    MapViewScene(MapViewer *pParent);

    inline void setSharedEditorData(SharedMapEditorDataPtr pSharedData) { m_pSharedData = pSharedData; }
    ConstSharedMapEditorDataPtr getSharedData() const;

signals:
    void brushPressed(MapViewer *pWidget, Point3D<uint32> point, uint32 uiButton);
    void brushReleased(MapViewer *pWidget, Point3D<uint32> point, uint32 uiButton);
    void brushMoved(MapViewer *pWidget, Point3D<uint32> point);

private:
    SharedMapEditorDataPtr m_pSharedData;
};

/*#####
# MapView
#####*/
class MapViewer : public ModifyView, public MapEditorObject
{
    Q_OBJECT
public:
    MapViewer(const MAP::MapPrototypePtr &map, SharedMapEditorDataPtr pDBMgr, QWidget *pParent = NULL);

    const MAP::MapPrototypePtr& getMap() const { return m_pMap; }
    inline MapViewScene* getScene() { return dynamic_cast<MapViewScene*>(scene()); }

    inline uint32 getCurrentLayer() const { return m_uiCurrentLayer; }
    inline void setCurrentLayer(uint32 uiCurrentLayer) { m_uiCurrentLayer = uiCurrentLayer; updateWidget(); }
    inline uint32 getMaxLayer(MAP::Layer layer) const { return m_pMap->getLayerSize(layer); }

    inline int32 getZoom() const { return m_CurZoom; }
    void setZoom(int32 zoom);

    void updateWidget();
    void changedMap();
    inline bool hasChanged() const { return m_hasChanges; }

    void saveCurrentMap();

    void updateText();

    void revertAction();
    void addAction(MAP::MapActionPtr pAction);

    inline void setShowGrid(bool show) { m_showGrid = show; updateWidget(); }
    inline bool getShowGrid() const { return m_showGrid; }

    void setObjectsEditable(bool editable = true);

signals:
    void textUpdated(MapViewer *pWidget, const QString &sTabName);

private:
    uint32 m_uiCurrentLayer;
    int m_CurZoom;
    bool m_showGrid;
    MAP::MapPrototypePtr m_pMap;
    bool m_hasChanges;
    MAP::MapActionPtrVector m_Actions;
};
#endif