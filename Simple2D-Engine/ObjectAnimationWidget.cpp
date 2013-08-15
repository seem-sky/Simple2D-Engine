#include "ObjectAnimationWidget.h"
#include "QtGlobal.h"
#include <QtWidgets/QGraphicsPixmapItem>

ObjectAnimationWidget::ObjectAnimationWidget(DATABASE::ConstAnimationPrototypePtr pAnimation, DATABASE::ConstSpriteDatabasePtr pSpriteDB, uint32 uiCurrentFrame) :
    QGraphicsView(), m_pSpriteDB(pSpriteDB)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background:transparent");
    setFrameShape(QFrame::NoFrame);
    setScene(new QGraphicsScene());
    DATABASE::AnimationPrototype::Frame frame;
    if (pAnimation->getFrame(uiCurrentFrame, frame))
        _drawFrame(frame);
}

void ObjectAnimationWidget::_drawFrame(const DATABASE::AnimationPrototype::Frame &frame)
{
    for (auto sprite : frame.m_Sprites)
    {
        DATABASE::ConstSpritePrototypePtr proto;
        QPixmap pixmap;
        if (m_pSpriteDB->getItem(sprite.m_uiSpriteID, proto) && createPixmapFromTexturePrototype("projects/untitled/", proto, pixmap))
        {
            QGraphicsPixmapItem *pItem = scene()->addPixmap(pixmap);
            pItem->setPos(QPoint(sprite.m_Pos.x, sprite.m_Pos.y));
            pItem->setScale(qreal(sprite.m_uiScale)/100);
            pItem->setRotation(sprite.m_uiRotation);
            pItem->setOpacity(qreal(sprite.m_uiOpacity)/100);
        }
    }
}