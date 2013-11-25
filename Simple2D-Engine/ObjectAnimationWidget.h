#ifndef OBJECT_ANIMATION_WIDGET_H
#define OBJECT_ANIMATION_WIDGET_H

#include "Database.h"
#include <QtWidgets/QGraphicsView>

class ObjectAnimationWidget : public QGraphicsView
{
private:
    void _drawFrame(const DATABASE::ANIMATION::Frame &frame);

public:
    ObjectAnimationWidget(const DATABASE::ANIMATION::AnimationPrototype *pAnimation, const DATABASE::SpriteDatabase *pSpriteDB, uint32 uiCurrentFrame);

private:
    const DATABASE::SpriteDatabase *m_pSpriteDB;
};
#endif