#ifndef TILE_VIEW_H
#define TILE_VIEW_H

#include <QtWidgets/QGraphicsView>
#include "DatabaseMgr.h"
#include "QtGlobal.h"
#include <QtWidgets/QGraphicsItem>
#include "MapEditorObject.h"

class TileItem : public QGraphicsItem
{
protected:
    void drawItem(QPainter *pPainter, ConstQPixmapPtr pPixmap);

public:
    TileItem(ConstSharedMapEditorDataPtr pSharedData, uint32 uiID);

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget = 0);
    QRectF boundingRect() const;

    inline uint32 getID() const { return m_uiID; }

protected:
    ConstSharedMapEditorDataPtr m_pSharedData;

private:
    uint32 m_uiID;
};

class AutoTileItem : public TileItem
{
public:
    AutoTileItem(ConstSharedMapEditorDataPtr pSharedData, uint32 uiID);

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget = 0);
};

class TileView : public QGraphicsView, public MapEditorObject
{
    Q_OBJECT
private:
    virtual void _updateWidget(int32 widgetWidth);

protected:
    void mousePressEvent(QMouseEvent *pEvent);

public:
    TileView(QWidget* pParent = NULL);

    void clearScene();
    void updateObject();

signals:
    void tileClicked(uint32 uiID, Qt::MouseButton button);
};

class AutoTileView : public TileView
{
private:
    void _updateWidget(int32 widgetWidth);

public:
    AutoTileView(QWidget* pParent = NULL);
};
#endif