#ifndef MODIFY_VIEW_H
#define MODIFY_VIEW_H

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsPixmapItem>
#include "Global.h"

class ModifyItem : public QGraphicsPixmapItem
{
protected:
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent);

    void sendUpdate();

public:
    ModifyItem();

    virtual void move(int32 x, int32 y);
};

class ModifyView : public QGraphicsView
{
    Q_OBJECT
public:
    ModifyView(QWidget *pParent = NULL) : QGraphicsView(pParent) {}

    void itemChanged(ModifyItem *pItem);

signals:
    void onItemChange(ModifyItem *pItem);
};
#endif