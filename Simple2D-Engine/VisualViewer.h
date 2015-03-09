#ifndef VISUAL_VIEWER_H
#define VISUAL_VIEWER_H

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtCore/QTimer>
#include "Database/Prototype/Derived.h"

namespace database
{
    class Manager;
}

class VisualSpriteItem : public QGraphicsItem
{
private:
    void _getPixmap(QPixmap& pixmap) const;

public:
    VisualSpriteItem(const database::Manager& DBMgr, uint32 ID);

    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = 0);

    uint32 getID() const;

    QRectF boundingRect() const;

private:
    uint32 m_ID;
    const database::Manager& m_DBMgr;
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
    void setDatabaseMgr(const database::Manager* pDBMgr);

    void clear();

    void showVisual();
    uint32 getCurrentFrame() const { return m_uiCurrentFrameIndex; }

	void setAnimation(const database::prototype::WorldObject::AnimationInfo& info);
    void setAnimation(uint32 animationEntry, database::prototype::WorldObject::VisualType type);
    inline uint32 getAnimationID() const { return m_AnimationEntry; }
    inline database::prototype::WorldObject::VisualType getVisualType() const { return m_VisualType; }

    virtual void startAnimation();
    virtual void stopAnimation();
    bool isAnimationActive() const;

	void showGrid(bool show = true);

    void resizeToContent();

private slots:
    void _onFrameExpired();

private:
    const database::Manager* m_pDBMgr;
    uint32 m_uiCurrentFrameIndex;
    QTimer m_AnimationTimer;

    uint32 m_AnimationEntry;
    database::prototype::WorldObject::VisualType m_VisualType;

    bool m_DoAnimation;
};

#endif
