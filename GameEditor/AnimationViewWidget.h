#ifndef ANIMATION_VIEW_WIDGET_H
#define ANIMATION_VIEW_WIDGET_H

#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include "QtGlobal.h"
#include "DatabaseChanger.h"
#include <QtCore/QTimer>
#include "ModifyView.h"

/*#####
# AnimationViewItem
#####*/
class AnimationViewItem : public ModifyItem
{
protected:
    void keyPressEvent(QKeyEvent *pEvent);
    void wheelEvent(QGraphicsSceneWheelEvent *pEvent);

public:
    AnimationViewItem(DATABASE::ConstSpritePrototypePtr pProto);

    inline uint32 getSpriteID() const { return m_uiID; }
    inline void setSpriteID(uint32 uiSpriteID) { m_uiID = uiSpriteID; }

private:
    uint32 m_uiID;
};

/*#####
# AnimationViewWidget
#####*/
class AnimationView : public ModifyView
{
    Q_OBJECT
private:
    bool _drawFrame(const DATABASE::AnimationPrototype::Frame &frame);

    virtual bool _getAnimationPrototype(uint32 uiID, DATABASE::ConstAnimationPrototypePtr &proto) = 0;
    virtual bool _getSpritePrototype(uint32 uiID, DATABASE::ConstSpritePrototypePtr &proto) = 0;

protected:
    void dragMoveEvent(QDragMoveEvent *pEvent);
    void dragEnterEvent(QDragEnterEvent *pEvent);
    void dropEvent(QDropEvent *pEvent);

public:
    AnimationView(QWidget *pParent = NULL);

    void fixSceneRect(bool fix = true);
    bool setCurrentFrame(uint32 uiFrame);

    void setCurrentAnimation(uint32 uiID);

    void createNewAnimationScene();

    void playCurrentAnimation();
    void stopCurrentAnimation();

    inline bool hasActiveAnimation() const { return m_ActiveAnimation; }
    inline void setContinuesAnimation(bool continues = true) { m_ContinuesAnimation = continues; }
    void setPreviousFrameDraw(bool draw = true);
    inline bool isPreviousFrameDrawActiv() const { return m_DrawPreviousFrame; }
    void setGridDraw(bool draw = true);
    inline bool isGridDrawActiv() const { return m_DrawGrid; }

    inline uint32 getCurrentAnimationID() const { return m_uiCurrentAnimationID; }

    inline void setSpeedModifyer(uint16 uiSpeedModifyer) { m_uiSpeedModifyer = uiSpeedModifyer; }
    inline uint16 getSpeedModifyer() const { return m_uiSpeedModifyer; }

    inline void setEditable(bool editable = true) { m_Editable = editable; }
    inline bool isEditable() const { return m_Editable; }

private slots:
    void _onUpdateAnimation(bool firstTime = false);

signals:
    void onAnimationStateChanged(bool started);
    void onDrop(AnimationView *pTarget, uint32 uiID, Point<int32> pos);
    void changedCurrentFrame(uint32 uiFrame, const DATABASE::AnimationPrototype::Frame &curFrame);

private:
    uint32 m_uiCurrentAnimationID;
    uint32 m_uiCurrentFrame;
    bool m_ContinuesAnimation;
    bool m_ActiveAnimation;
    QTimer m_Timer;
    bool m_DrawGrid;
    bool m_DrawPreviousFrame;
    uint16 m_uiSpeedModifyer;
    bool m_Editable;
};

class AnimationViewDBChanger : public AnimationView
{
private:
    bool _getAnimationPrototype(uint32 uiID, DATABASE::ConstAnimationPrototypePtr &proto);
    bool _getSpritePrototype(uint32 uiID, DATABASE::ConstSpritePrototypePtr &proto);

public:
    AnimationViewDBChanger(QWidget *pParent = NULL) : AnimationView(pParent)
    {}

    inline void setAnimationDB(const DATABASE::ConstAnimationDatabaseChangerPtr &pDB) { m_pAnimationDB = pDB; }
    inline void setSpriteDB(const DATABASE::ConstSpriteDatabaseChangerPtr &pDB) { m_pSpriteDB = pDB; }

private:
    DATABASE::ConstAnimationDatabaseChangerPtr m_pAnimationDB;
    DATABASE::ConstSpriteDatabaseChangerPtr m_pSpriteDB;
};

class AnimationViewDB : public AnimationView
{
private:
    bool _getAnimationPrototype(uint32 uiID, DATABASE::ConstAnimationPrototypePtr &proto);
    bool _getSpritePrototype(uint32 uiID, DATABASE::ConstSpritePrototypePtr &proto);

public:
    AnimationViewDB(QWidget *pParent = NULL) : AnimationView(pParent)
    {}

    inline void setAnimationDB(const DATABASE::ConstAnimationDatabasePtr &pDB) { m_pAnimationDB = pDB; }
    inline void setSpriteDB(const DATABASE::ConstSpriteDatabasePtr &pDB) { m_pSpriteDB = pDB; }

private:
    DATABASE::ConstAnimationDatabasePtr m_pAnimationDB;
    DATABASE::ConstSpriteDatabasePtr m_pSpriteDB;
};

/*#####
# AnimationViewScene
#####*/
class AnimationViewScene : public QGraphicsScene
{
private:
    void _drawPreviousFrame(QPainter *painter, const QRectF &rect);
    void _drawGrid(QPainter *painter, const QRectF &rect);

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

public:
    AnimationViewScene(AnimationView *pParent = NULL);
    inline void setPreviousFrame(const DATABASE::AnimationPrototype::Frame &frame) { m_PreviousFrame = frame; }
    AnimationViewItem* getSelectedItem();

    inline void setGridDraw(bool draw = true) { m_DrawGrid = draw; }
    inline bool isGridDrawActiv() const { return m_DrawGrid; }
    inline bool isPreviousFrameDrawActiv() const { return m_DrawPreviousFrame; }
    inline void setPreviousFrameDraw(bool draw = true) { m_DrawPreviousFrame = draw; }

private:
    DATABASE::AnimationPrototype::Frame m_PreviousFrame;
    bool m_DrawGrid;
    bool m_DrawPreviousFrame;
};
#endif