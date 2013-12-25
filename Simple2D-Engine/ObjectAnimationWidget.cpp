#include "ObjectAnimationWidget.h"
#include "QtGlobal.h"
#include <QtWidgets/QGraphicsPixmapItem>

ObjectAnimationWidget::ObjectAnimationWidget(const DATABASE::ANIMATION::AnimationPrototype* pAnimation, const DATABASE::SpriteDatabase* pSpriteDB, uint32 uiCurrentFrame) :
    QGraphicsView(), m_pSpriteDB(pSpriteDB)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background:transparent");
    setFrameShape(QFrame::NoFrame);
    setScene(new QGraphicsScene());
    DATABASE::ANIMATION::Frame frame;
    if (pAnimation->getFrame(uiCurrentFrame, frame))
        _drawFrame(frame);
}

void ObjectAnimationWidget::_drawFrame(const DATABASE::ANIMATION::Frame& frame)
{
    //for (const auto& sprite : frame.getSprites())
    //{
    //    QPixmap pixmap;
    //    auto pSprite = m_pSpriteDB->getOriginalPrototype(sprite.m_uiSpriteID);
    //    if (pSprite& & createPixmapFromTexturePrototype("projects/untitled/", pSprite, pixmap))
    //    {
    //        auto* pItem = scene()->addPixmap(pixmap);
    //        pItem->setPos(QPoint(sprite.m_Pos.x, sprite.m_Pos.y));
    //        pItem->setScale(qreal(sprite.m_uiScale)/100);
    //        pItem->setRotation(sprite.m_uiRotation);
    //        pItem->setOpacity(qreal(sprite.m_uiOpacity)/100);
    //    }
    //}
}