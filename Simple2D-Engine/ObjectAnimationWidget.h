#ifndef OBJECT_ANIMATION_WIDGET_H
#define OBJECT_ANIMATION_WIDGET_H

#include "Database.h"
#include <QtWidgets/QGraphicsView>

class ObjectAnimationWidget : public QGraphicsView
{
private:
    void _drawFrame(const DATABASE::PROTOTYPE::ANIMATION::Frame& frame);

public:
    ObjectAnimationWidget(const DATABASE::PROTOTYPE::ANIMATION::AnimationPrototype* pAnimation, const DATABASE::SpriteDatabase* pSpriteDB, uint32 uiCurrentFrame);

private:
    const DATABASE::SpriteDatabase* m_pSpriteDB;
};
#endif