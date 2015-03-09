#ifndef GRAPHICS_SPRITE_ITEM_H
#define GRAPHICS_SPRITE_ITEM_H

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <Global.h>

class GraphicsSpriteItem : public QGraphicsItem
{
private:
    virtual QPixmap _getPixmap() const = 0;

protected:
    virtual void keyPressEvent(QKeyEvent* pEvent);

public:
    GraphicsSpriteItem(uint32 ID);

    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);

    uint32 getID() const;

    QRectF boundingRect() const;

private:
    uint32 m_ID;
};
#endif
