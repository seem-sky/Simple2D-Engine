#ifndef ANIMATION_VIEWER_WIDGET_H
#define ANIMATION_VIEWER_WIDGET_H

#include "Ui/UI_AnimationViewerWidget.h"
#include <QtWidgets/QTableWidget>
#include "AnimationViewWidget.h"
#include "DragTreeWidget.h"

/*#####
# ObjectAnimationViewScene
#####*/
class ObjectAnimationViewScene : public AnimationViewScene
{
protected:
    void drawForeground(QPainter *painter, const QRectF &rect);

public:
    ObjectAnimationViewScene(AnimationView *pParent = NULL);

    inline void setBoundingRect(const QRect &boundingRect) { m_BoundingRect = boundingRect; }

private:
    QRect m_BoundingRect;
};

/*#####
# AnimationViewerTableWidget
#####*/
class AnimationViewerWidget;
class AnimationViewerTableWidget: public QTableWidget
{
    Q_OBJECT
private:
    void _setupAnimationViewerComboBox(AnimationViewerWidget *pWidget);
    void _resizeToShortestColumn(uint32 lastIndex);
    void _resizeIfNeeded(uint32 uiIndex);
    void _setAnimationWidget(uint32 uiIndex, AnimationViewerWidget *pWidget);
    ObjectAnimationTypeTreeWidget* _getNewObjectAnimationTypeTreeWidget();

public:
    AnimationViewerTableWidget(QWidget *pParent = NULL);

    AnimationViewerWidget* getNewAnimationViewerWidget();
    void setAdditionalDBs(DATABASE::ConstSpriteDatabaseChangerPtr pSpriteDB, DATABASE::ConstAnimationDatabaseChangerPtr pAnimationDB,
        DATABASE::ConstObjectAnimationTypeDatabaseChangerPtr pObjAniTypeDB);

    void setCellAnimationInfo(uint32 uiRow, DATABASE::WorldObjectPrototype::AnimationInfo animationInfo);
    DATABASE::WorldObjectPrototype::AnimationInfo getCellAnimationInfo(uint32 uiRow);

    void setSpeedModifyer(uint32 uiSpeedModifyer);
    void setSceneBoundingRect(const QRect &boundingRect);

    void setAnimationWidget(uint32 index, AnimationViewerWidget *pWidget);
    AnimationViewerWidget* getAnimationWidget(uint32 index);
    void removeAnimationWidget(uint32 index);

    Int32Point getCellFromIndex(uint32 uiIndex);

    uint32 getCellCount() const { return m_uiSize; }

signals:
    void onItemAdd(AnimationViewerWidget *pWidget);
    void onItemRemove(AnimationViewerWidget *pWidget);

private:
    DATABASE::ConstObjectAnimationTypeDatabaseChangerPtr m_pObjAniTypeDB;
    DATABASE::ConstSpriteDatabaseChangerPtr m_pSpriteDB;
    DATABASE::ConstAnimationDatabaseChangerPtr m_pAnimationDB;

    ObjectAnimationTypeTreeWidget m_ObjectAnimationTypeTreeWidget;

    uint32 m_uiSize;
};

/*#####
# AnimationViewerWidget
#####*/
class AnimationViewerWidget : public QWidget, public Ui_AnimationViewerWidget
{
    Q_OBJECT
public:
    AnimationViewerWidget(AnimationViewerTableWidget *pParent = NULL);

    inline uint32 getCurrentObjectAnimationType() const { return m_uiObjectAnimationType; }
    void setCurrentObjectAnimationType(uint32 uiID);

private slots:
    void _comboBoxIndexChanged(int index);

signals:
    void onItemChanged();

private:
    uint32 m_uiObjectAnimationType;
};
#endif