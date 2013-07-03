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
    MapObjectItem(DATABASE::MAP::MapObjectPtr pMapObj) : ModifyItem(), m_pMapObject(pMapObj)
    {}

    void move(int x, int y);

    void updateItemPixmap();

private:
    DATABASE::MAP::MapObjectPtr m_pMapObject;
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
    void _drawTiles(QPainter *painter, const QRectF &rect, DATABASE::MAP::Layer layer);

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

    inline void emitItemChanged(MapObjectItem *pItem) { emit itemChanged(pItem); }

signals:
    void brushPressed(MapViewer *pWidget, UInt32Point3D point, uint32 uiButton);
    void brushReleased(MapViewer *pWidget, UInt32Point3D point, uint32 uiButton);
    void brushMoved(MapViewer *pWidget, UInt32Point3D point);

    void itemChanged(MapObjectItem *pItem);

private:
    SharedMapEditorDataPtr m_pSharedData;
};

/*#####
# MapView
#####*/
class MapViewer : public ModifyView, public MapEditorObject
{
    Q_OBJECT
private:
    void _placeMapObjects();

public:
    MapViewer(const DATABASE::MAP::MapPrototypePtr &map, SharedMapEditorDataPtr pDBMgr, QWidget *pParent = NULL);

    const DATABASE::MAP::MapPrototypePtr& getMap() const { return m_pMap; }
    inline MapViewScene* getScene() { return dynamic_cast<MapViewScene*>(scene()); }

    inline uint32 getCurrentLayer(DATABASE::MAP::Layer layer) const { return m_uiCurrentLayer[layer]; }
    inline void setCurrentLayer(uint32 uiCurrentLayer, DATABASE::MAP::Layer layer) { m_uiCurrentLayer[layer] = uiCurrentLayer; redrawViewport(); }
    uint32 getMaxLayer(DATABASE::MAP::Layer layer) const { return m_pMap->getLayerSize(layer); }

    inline DATABASE::MAP::Layer getLayer() const { return m_Layer; }
    inline void setLayer(DATABASE::MAP::Layer layer) { m_Layer = layer; }

    inline int32 getZoom() const { return m_CurZoom; }
    void setZoom(int32 zoom);

    void redrawViewport();
    void updateObject();
    void changedMap();
    inline bool hasChanged() const { return m_hasChanges; }

    void saveCurrentMap();

    void updateText();

    void revertAction();
    void addAction(DATABASE::MAP::MapActionPtr pAction);

    inline void setShowGrid(bool show) { m_showGrid = show; redrawViewport(); }
    inline bool getShowGrid() const { return m_showGrid; }

    void setObjectsEditable(bool editable = true);

private slots:
    void _onItemChange(MapObjectItem *pItem);

signals:
    void textUpdated(MapViewer *pWidget, const QString &sTabName);

private:
    uint32 m_uiCurrentLayer[2];
    DATABASE::MAP::Layer m_Layer;
    int m_CurZoom;
    bool m_showGrid;
    DATABASE::MAP::MapPrototypePtr m_pMap;
    bool m_hasChanges;
    DATABASE::MAP::MapActionPtrVector m_Actions;
};
#endif