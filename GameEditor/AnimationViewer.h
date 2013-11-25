#ifndef ANIMATION_VIEWER_H
#define ANIMATION_VIEWER_H

#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QtCore/QTimer>
#include "Database.h"

class GraphicsSpriteItem : public QGraphicsItem
{
private:
    QPixmap _getPixmap() const;
    void _hightlightSelection(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void keyPressEvent(QKeyEvent *pEvent);

public:
    GraphicsSpriteItem(const DATABASE::SpritePrototype *pSpritePrototype);

    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget = 0);

    uint32 getSpriteID() const;

    DATABASE::ANIMATION::Sprite toSprite() const;

    QRectF boundingRect() const;

private:
    const DATABASE::SpritePrototype *m_pSpritePrototype;
};

class AnimationViewerScene : public QGraphicsScene
{
    Q_OBJECT
private:
    void _drawGrid(QPainter *pPainter, const QRectF &rect);

protected:
    void drawForeground(QPainter *pPainter, const QRectF &rect);

public:
    AnimationViewerScene();

    void showGrid(bool show);

signals:
    void itemChanged(GraphicsSpriteItem *pItem, QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    void itemAdded(GraphicsSpriteItem *pItem);
    void itemRemoved(GraphicsSpriteItem *pItem);

private:
    bool m_DrawGrid;
};

class AnimationViewer : public QGraphicsView
{
    Q_OBJECT
private:
    void _setupFrame(const DATABASE::ANIMATION::Frame &frame);
    void _setupTimer();

protected:
    void dragMoveEvent(QDragMoveEvent *pEvent);
    void dragEnterEvent(QDragEnterEvent *pEvent);
    void dropEvent(QDropEvent *pEvent);

public:
    AnimationViewer(QWidget *pParent = nullptr);

    void clear();

    void addGraphicsSpriteItem(GraphicsSpriteItem *pItem);

    void setSpriteDatabase(const DATABASE::SpriteDatabase *pSpriteDB);
    void setAnimation(const DATABASE::ANIMATION::FrameVector *pAnimation);

    void showFrame(uint32 index);
    uint32 getCurrentFrame() const { return m_uiCurrentFrameIndex; }

    void startAnimation();
    void stopAnimation();
    bool playAnimation() const;

    GraphicsSpriteItem* getSelectedItem();

    enum Mode
    {
        MODE_VIEW,
        MODE_MODIFY
    };
    void setMode(Mode mode);

private slots:
    void _onFrameExpired();

private:
    Mode m_Mode;
    QTimer m_AnimationTimer;
    uint32 m_uiCurrentFrameIndex;
    const DATABASE::SpriteDatabase *m_pSpriteDB;
    const DATABASE::ANIMATION::FrameVector *m_pAnimation;
};

#endif
