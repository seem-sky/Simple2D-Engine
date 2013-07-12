#ifndef OBJECT_ANIMATION_WIDGET_H
#define OBJECT_ANIMATION_WIDGET_H

#include "Database.h"
#include <QtWidgets/QGraphicsView>

class ObjectAnimationWidget : public QGraphicsView
{
private:
    void _drawFrame(const DATABASE::AnimationPrototype::Frame &frame);

public:
    ObjectAnimationWidget(DATABASE::ConstAnimationPrototypePtr pAnimation, DATABASE::ConstSpriteDatabasePtr pSpriteDB, uint32 uiCurrentFrame);

private:
    DATABASE::ConstSpriteDatabasePtr m_pSpriteDB;
};
#endif