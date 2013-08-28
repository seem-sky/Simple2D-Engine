#ifndef ANIMATION_DATABASE_WIDGET_H
#define ANIMATION_DATABASE_WIDGET_H

#include "DatabaseWidget.h"
#include "Ui/UI_AnimationWidget.h"

class AnimationDatabaseWidget : public DatabaseWidget<DATABASE::AnimationPrototype, DATABASE::ANIMATION_INDEX>, Ui_AnimationWidget
{
    Q_OBJECT
private:
    void _updateSelectedSprite();
    AnimationViewScene* _createNewAnimationViewScene();
    void _setCurrentFrameValue(uint32 uiFrame);
    void _fillSprites();
    bool getItemFromWidgets(DATABASE::AnimationPrototypePtr &proto);
    bool setWidgetsFromPrototype(const DATABASE::AnimationPrototypePtr &proto);

public:
    AnimationDatabaseWidget(QWidget *pParent = NULL);
    void setDB(const DATABASE::AnimationDatabasePtr &pDB);
    inline void setSpriteDB(DATABASE::ConstSpriteDatabaseChangerPtr pSpriteDB)
    {
        m_pSpriteDB = pSpriteDB;
        m_pAniViewer->setSpriteDB(pSpriteDB);
        m_pSpriteList->setDB(m_pSpriteDB);
    }
    inline DATABASE::ConstSpriteDatabaseChangerPtr getSpriteDB() const { return m_pSpriteDB; }
    void setFocus();

private slots:
    void _onAnimationStateChanged(bool started);
    void _currentSpriteModifyed(int value);
    void _sceneSelectionChanged();
    void _onAnimationPlayerClicked();
    void _onSpriteChange(ModifyItem *pItem);
    void _onFrameTimeChange();
    void _onFrameChange(int value);
    void _onFrameChange(uint32 uiFrame, const DATABASE::AnimationPrototype::Frame &curFrame);
    void _onDrop(AnimationView *pTarget, uint32 uiID, Int32Point pos);

    DATABASE_WIDGET_SLOTS

private:
    DATABASE::ConstSpriteDatabaseChangerPtr m_pSpriteDB;
};
#endif