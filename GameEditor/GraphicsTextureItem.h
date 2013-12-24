#ifndef GRAPHICS_SPRITE_ITEM_H
#define GRAPHICS_SPRITE_ITEM_H

#include "Database.h"
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>

class GraphicsTextureItem : public QGraphicsItem
{
private:
    QPixmap _getPixmap() const;
    void _hightlightSelection(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption);

protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);

public:
    GraphicsTextureItem(const DATABASE::TexturePrototype *pPrototype);

    virtual void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget = 0);

    uint32 getID() const;

    QRectF boundingRect() const;

private:
    const DATABASE::TexturePrototype *m_pPrototype;
};

#endif
