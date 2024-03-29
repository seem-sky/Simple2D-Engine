#ifndef ANIMATION_VIEWER_H
#define ANIMATION_VIEWER_H

#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtCore/QTimer>
#include "GraphicsTextureItem.h"
#include "Database/Derived.h"
#include "Animation/FrameData.h"

class AnimationSpriteItem : public GraphicsSpriteItem
{
private:
    QPixmap _getPixmap() const;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    void keyPressEvent(QKeyEvent* pEvent);

public:
    AnimationSpriteItem(const database::SpriteDatabase* pSpriteDB, uint32 ID);

    animation::SpriteData toSprite() const;

private:
    const database::SpriteDatabase* m_pSpriteDB;
};

class AnimationViewerScene : public QGraphicsScene
{
    Q_OBJECT
private:
    void _drawGrid(QPainter* pPainter, const QRectF& rect);

protected:
    void drawForeground(QPainter* pPainter, const QRectF& rect);

public:
    AnimationViewerScene();

    void showGrid(bool show);

signals:
    void itemChanged(AnimationSpriteItem* pItem, QGraphicsItem::GraphicsItemChange change, const QVariant& value);
    void itemAdded(AnimationSpriteItem* pItem);
    void itemRemoved(AnimationSpriteItem* pItem);

private:
    bool m_DrawGrid;
};

class AnimationViewer : public QGraphicsView
{
    Q_OBJECT
private:
    void _setupFrame(const animation::FrameData& frame);
    void _setupTimer();

protected:
    void dragMoveEvent(QDragMoveEvent* pEvent);
    void dragEnterEvent(QDragEnterEvent* pEvent);
    void dropEvent(QDropEvent* pEvent);

public:
    AnimationViewer(QWidget* pParent = nullptr);

    void clear();

    void addGraphicsSpriteItem(AnimationSpriteItem* pItem);

    void setSpriteDatabase(const database::SpriteDatabase *pSpriteDB);
    void setAnimation(const animation::FrameVector* pAnimation);

    void showFrame(uint32 index);
    uint32 getCurrentFrame() const { return m_uiCurrentFrameIndex; }

    void startAnimation();
    void stopAnimation();
    bool isAnimationActive() const;

    AnimationSpriteItem* getSelectedItem();

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
    const database::SpriteDatabase* m_pSpriteDB;
    const animation::FrameVector* m_pAnimation;
};
#endif
