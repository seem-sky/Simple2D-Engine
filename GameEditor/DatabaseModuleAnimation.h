#ifndef DATABASE_MODULE_ANIMATION_H
#define DATABASE_MODULE_ANIMATION_H

#include "UI/UI_DatabaseModuleAnimation.h"
#include "Database.h"

class AnimationAction
{
public:
    AnimationAction(AnimationSpriteItem *pItem, AnimationViewerScene *pScene);
    virtual void revert() = 0;

protected:
    AnimationViewerScene *m_pScene;
    AnimationSpriteItem *m_pItem;
};
typedef std::vector<std::unique_ptr<AnimationAction>> AnimationActionPtrVector;

class AnimationActionInsert : public AnimationAction
{
public:
    void revert();
};

class AnimationActionRemove : public AnimationAction
{
public:
    ~AnimationActionRemove();

    void revert();
};

class AnimationActionModify : public AnimationAction
{
public:
    AnimationActionModify(QGraphicsItem::GraphicsItemChange change, const QVariant &value, AnimationSpriteItem *pItem, AnimationViewerScene *pScene);
    void revert();

private:
    QGraphicsItem::GraphicsItemChange m_ItemChange;
    const QVariant m_Value;
};

class DatabaseModuleAnimation : public QWidget, Ui_DatabaseModuleAnimation
{
    Q_OBJECT
private:
    void _setupSpriteFrame(AnimationSpriteItem *pItem);

    void _saveCurrentFrame();
    void _setupFrame(uint32 index);

public:
    DatabaseModuleAnimation(QWidget *pParent = nullptr);

    inline const DATABASE::ANIMATION::FrameVector& getAnimation() const { return m_Animation; }
    void setAnimation(const DATABASE::ANIMATION::FrameVector &animation);

    void setSpriteDatabase(const DATABASE::SpriteDatabase *pSpriteDB);

    void clear();

private slots:
    void _onGridCheckboxClicked(bool checked);
    void _onPlayStopButtonClicked();
    void _onFrameChanged(int index);
    void _onSelectionChanged();
    void _onItemChanged(AnimationSpriteItem *pItem, QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    //void _onActionRevert();

    // sprite slots
    void _onXChanged(int value);
    void _onYChanged(int value);
    void _onZValueChanged(int value);
    void _onScaleChanged(double value);
    void _onRotationChanged(int value);
    void _onOpacityChanged(double value);

private:
    DATABASE::ANIMATION::FrameVector m_Animation;
    const DATABASE::SpriteDatabase *m_pSpriteDB;
    AnimationActionPtrVector m_LastActions;
};

#endif
