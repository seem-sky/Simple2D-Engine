#ifndef SCRIPT_AREA_ITEM_H
#define SCRIPT_AREA_ITEM_H

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPen>
#include <Typedefs.h>

namespace MAP
{
    namespace SCRIPT_AREA
    {
        class ScriptArea;
    }
}

class PointMoveItem : public QGraphicsItem
{
public:
    PointMoveItem(uint32 index, QGraphicsItem* pItem = nullptr);

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);
    QRectF boundingRect() const;

    const QPen& getPen() const { return m_Pen; }
    void setPen(const QPen& pen);

    int type() const;
    uint32 getIndex() const { return m_Index; }

private:
    QPen m_Pen;
    uint32 m_Index = 0;
};

class ScriptAreaItem : public QGraphicsItem
{
private:
    void _setupPointMoveItem(uint32 index);
    void _updateEdgePositions();

protected:
    bool sceneEventFilter(QGraphicsItem* pItem, QEvent* pEvent);

public:
    ScriptAreaItem();

    void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);
    QRectF boundingRect() const;

    void setup(MAP::SCRIPT_AREA::ScriptArea* scriptArea);

    const MAP::SCRIPT_AREA::ScriptArea* getScriptArea() const;
    void setEditable(bool editable = true);

    PointMoveItem* getPointItem(uint32 index) const;

    int type() const;

private:
    MAP::SCRIPT_AREA::ScriptArea* m_pScriptArea = nullptr;
};
#endif
