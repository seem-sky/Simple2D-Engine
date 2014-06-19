#ifndef VISUAL_VIEWER_H
#define VISUAL_VIEWER_H

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QTimer>
#include "DatabaseMgr.h"
#include "PrototypeAnimationModule.h"

class VisualSpriteItem : public QGraphicsItem
{
private:
    void _getPixmap(QPixmap& pixmap) const;

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
    void _setupSprite() const;
    int _setupAnimationFrame(uint32 frameIndex);

protected:
    void dragMoveEvent(QDragMoveEvent* pEvent);
    void dragEnterEvent(QDragEnterEvent* pEvent);
    virtual void dropEvent(QDropEvent* pEvent);

public:
    VisualViewer(QWidget *pParent = nullptr);
    void setDatabaseManager(const DATABASE::DatabaseMgr* pDBMgr);

    void clear();

    void showVisual();
    uint32 getCurrentFrame() const { return m_uiCurrentFrameIndex; }

    void setAnimation(uint32 animationEntry, DATABASE::PROTOTYPE::MODULE::ANIMATION::VisualType type);
    inline uint32 getAnimationID() const { return m_AnimationEntry; }
	inline DATABASE::PROTOTYPE::MODULE::ANIMATION::VisualType getVisualType() const { return m_VisualType; }

    virtual void startAnimation();
    virtual void stopAnimation();
    bool isAnimationActive() const;

private slots:
    void _onFrameExpired();

private:
    const DATABASE::DatabaseMgr* m_pDBMgr;
    uint32 m_uiCurrentFrameIndex;
    QTimer m_AnimationTimer;

    uint32 m_AnimationEntry;
	DATABASE::PROTOTYPE::MODULE::ANIMATION::VisualType m_VisualType;

    bool m_DoAnimation;
};

#endif
