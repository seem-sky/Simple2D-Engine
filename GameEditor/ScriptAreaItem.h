#ifndef SCRIPT_AREA_ITEM_H
#define SCRIPT_AREA_ITEM_H

#include <QtWidgets/QGraphicsObject>
#include <QtGui/QPen>
#include <Typedefs.h>
#include <map>
#include "Simple2D-Global.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        class ScriptArea;
    }
}

class PointMoveItem : public QGraphicsObject
{
    Q_OBJECT
private:
    void _posModifyStart(int key);
    void _posModifyEnd(int key);

protected:
    void keyPressEvent(QKeyEvent *pEvent);
    void keyReleaseEvent(QKeyEvent *pEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *pEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *pEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent);

public:
    PointMoveItem(uint32 index, QGraphicsItem* pItem = nullptr);

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);
    QRectF boundingRect() const;

    const QPen& getPen() const { return m_Pen; }
    void setPen(const QPen& pen);

    int type() const;
    uint32 getIndex() const { return m_Index; }

signals:
    void positionChanged(uint32 index, QPoint m_Pos);
    void positionChanged(uint32 index);

private:
    QPen m_Pen;
    uint32 m_Index = 0;
    std::map<int, QPoint> m_Pos;
};

class ScriptAreaItem : public QGraphicsObject
{
    Q_OBJECT
private:
    void _setupPointMoveItem(uint32 index);
    void _updateEdgePositions();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);

public:
    ScriptAreaItem();

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);
    QRectF boundingRect() const;

    void setup(MAP::SCRIPT_AREA::ScriptArea* scriptArea);

    const MAP::SCRIPT_AREA::ScriptArea* getScriptArea() const;
    MAP::SCRIPT_AREA::ScriptArea* getScriptArea();
    void setEditable(bool editable = true);

    PointMoveItem* getPointItem(uint32 index) const;
    void setPoint(uint32 index, const GEOMETRY::Point<int32>& pos);

    int type() const;

private slots:
    void _childPositionChanged(uint32 index, QPoint pos);
    void _childPositionChanged(uint32 index);

signals:
    void modified(MAP::GUID guid, uint32 index, QPoint pos);

private:
    MAP::SCRIPT_AREA::ScriptArea* m_pScriptArea = nullptr;
};
#endif
