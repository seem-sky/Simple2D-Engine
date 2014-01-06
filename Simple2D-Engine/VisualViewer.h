#ifndef VISUAL_VIEWER_H
#define VISUAL_VIEWER_H

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QTimer>
#include "DatabaseMgr.h"

class VisualSpriteItem : public QGraphicsItem
{
private:
    QPixmap _getPixmap() const;

public:
    VisualSpriteItem(const DATABASE::DatabaseMgr& DBMgr, uint32 ID);

    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);

    uint32 getID() const;

    QRectF boundingRect() const;

private:
    uint32 m_ID;
    const DATABASE::DatabaseMgr& m_DBMgr;
};

class VisualViewerScene : public QGraphicsScene
{
private:
    void _drawGrid(QPainter* pPainter, const QRectF& rect);

protected:
    void drawForeground(QPainter* pPainter, const QRectF& rect);

public:
    VisualViewerScene();

    void showGrid(bool show);

private:
    bool m_DrawGrid;
};

class VisualViewer : public QGraphicsView
{
    Q_OBJECT
private:
    void _setupSprite(uint32 ID) const;
    void _setupAnimationFrame(uint32 ID, uint32 frameIndex);

protected:
    void dragMoveEvent(QDragMoveEvent* pEvent);
    void dragEnterEvent(QDragEnterEvent* pEvent);
    virtual void dropEvent(QDropEvent* pEvent);

public:
    VisualViewer(const DATABASE::DatabaseMgr& DBMgr, QWidget *pParent = nullptr);

    void clear();

    void showVisual();
    uint32 getCurrentFrame() const { return m_uiCurrentFrameIndex; }

    DATABASE::WORLD_OBJECT::AnimationInfo getAnimationInfo() const;

    virtual void startAnimation();
    virtual void stopAnimation();
    bool isAnimationActive() const;

private slots:
    void _onFrameExpired();

private:
    const DATABASE::DatabaseMgr& m_DBMgr;
    uint32 m_uiCurrentFrameIndex;
    QTimer m_AnimationTimer;

    uint32 m_WorldObjectID;
    uint32 m_AnimationEntry;

    bool m_DoAnimation;
};

#endif
