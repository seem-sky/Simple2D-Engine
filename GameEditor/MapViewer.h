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
    MapObjectItem(DATABASE::MAP_STRUCTURE::MapObject *pMapObj) : ModifyItem(), m_pMapObject(pMapObj)
    {}

    void move(int x, int y);

    void updateItemPixmap();

    inline uint32 getGUID() const { return m_pMapObject->m_GUID; }

private:
    DATABASE::MAP_STRUCTURE::MapObject *m_pMapObject;
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
    void _drawTiles(QPainter *painter, const QRectF &rect, DATABASE::MAP_STRUCTURE::Layer layer);

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
    MapViewer(const DATABASE::MAP_STRUCTURE::MapPrototype *map, SharedMapEditorDataPtr pDBMgr, QWidget *pParent = NULL);

    const DATABASE::MAP_STRUCTURE::MapPrototype* getMap() const { return m_pMap; }
    inline MapViewScene* getScene() { return dynamic_cast<MapViewScene*>(scene()); }

    inline uint32 getCurrentLayer(DATABASE::MAP_STRUCTURE::Layer layer) const { return m_uiCurrentLayer[layer]; }
    inline void setCurrentLayer(uint32 uiCurrentLayer, DATABASE::MAP_STRUCTURE::Layer layer) { m_uiCurrentLayer[layer] = uiCurrentLayer; redrawViewport(); }
    uint32 getMaxLayer(DATABASE::MAP_STRUCTURE::Layer layer) const { return m_pMap->getLayerSize(layer); }

    inline DATABASE::MAP_STRUCTURE::Layer getLayer() const { return m_Layer; }
    inline void setLayer(DATABASE::MAP_STRUCTURE::Layer layer) { m_Layer = layer; }

    inline int32 getZoom() const { return m_CurZoom; }
    void setZoom(int32 zoom);

    void redrawViewport();
    void updateObject();
    void changedMap();
    inline bool hasChanged() const { return m_hasChanges; }

    void saveCurrentMap();

    void updateText();

    void revertAction();
    void addAction(DATABASE::MAP_STRUCTURE::MapActionPtr pAction);

    inline void setShowGrid(bool show) { m_showGrid = show; redrawViewport(); }
    inline bool getShowGrid() const { return m_showGrid; }

    void setObjectsEditable(bool editable = true);
    void setObjectEditable(QGraphicsItem *pItem, bool editable = true);

    void itemRemoved(ModifyItem *pItem);

private slots:
    void _onItemChange(MapObjectItem *pItem);

signals:
    void textUpdated(MapViewer *pWidget, const QString &sTabName);

private:
    std::array<uint32, 2> m_uiCurrentLayer;
    DATABASE::MAP_STRUCTURE::Layer m_Layer;
    int m_CurZoom;
    bool m_showGrid;
    DATABASE::MAP_STRUCTURE::MapPrototype *m_pMap;
    bool m_hasChanges;
    DATABASE::MAP_STRUCTURE::MapActionPtrVector m_Actions;
};
#endif