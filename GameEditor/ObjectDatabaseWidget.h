#ifndef OBJECT_DATABASE_WIDGET_H
#define OBJECT_DATABASE_WIDGET_H

#include "DatabaseWidget.h"
#include "Ui/UI_WorldObjectDatabaseWidget.h"

#define OBJECT_DATABASE_WIDGET_SLOTS void _animationSpeedChanged() { updateAnimationSpeed(); }\
    void _boundingRectChanged(int value) { updateBoundingRect(); }\
    void _onRemoveAnimationButtonClicked() { removeAnimation(); }\
    void _onAddAnimationButtonClicked() { addAnimation(); }\
    void _onDrop(AnimationView *pTarget, uint32 uiID, Int32Point pos) { dropAnimation(pTarget, uiID, pos); }\
    void _onItemChanged() { updateItem(); }\
    void _onItemAdded(AnimationViewerWidget *pWidget) { addedItem(pWidget); }\
    DATABASE_WIDGET_SLOTS

template <class T>
class ObjectDatabaseWidget : public DatabaseWidget<T>, public Ui_WorldObjectDatabaseWidget
{
protected:
    void setupAnimationViewer(AnimationView* pViewer, uint32 uiAnimationID)
    {
        if (!pViewer)
            return;
        pViewer->setCurrentAnimation(uiAnimationID);
        pViewer->setContinuesAnimation();
        pViewer->playCurrentAnimation();
    }

    void updateAnimationSpeed()
    {
        updateItem();
        m_pAnimationTable->setSpeedModifyer(m_pSpeedModifyer->value());
    }

    void updateBoundingRect()
    {
        updateItem();
        m_pAnimationTable->setSceneBoundingRect(getBoundingRect());
    }

    // animation view table
    void addedItem(AnimationViewerWidget *pWidget)
    {
        connect(pWidget, SIGNAL(onItemChanged()), this, SLOT(_onItemChanged()));
    }

    void addAnimation()
    {
        AnimationViewerWidget *pWidget = m_pAnimationTable->getNewAnimationViewerWidget();
        m_pAnimationTable->setAnimationWidget(m_pAnimationTable->getCellCount(), pWidget);
        if (ObjectAnimationViewScene *pScene = dynamic_cast<ObjectAnimationViewScene*>(pWidget->m_pAnimationView->scene()))
            pScene->setBoundingRect(getBoundingRect());
        updateItem();
    }

    void removeAnimation()
    {
        if (m_pAnimationTable->getCellCount() <= 4)
            return;
        m_pAnimationTable->removeAnimationWidget(m_pAnimationTable->getCellCount()-1);
        updateItem();
    }

    void dropAnimation(AnimationView *pTarget, uint32 uiID, Int32Point pos)
    {
        setupAnimationViewer(pTarget, uiID);
        updateItem();
    }

public:
    ObjectDatabaseWidget(QWidget *pParent = NULL) : DatabaseWidget(pParent)
    {
        Ui_WorldObjectDatabaseWidget::setupUi(this);
        m_ModObj.setWidget(m_pAnimationList, MODIFY_RESIZE, QPoint(5, 5), MODIFY_DIRECTION_HEIGHT);
        m_ModObj.setWidget(m_pAnimationLabel, MODIFY_MOVE, QPoint(m_pAnimationList->width() - m_pAnimationLabel->width() + 5, 5), MODIFY_DIRECTION_WIDTH);
        m_ModObj.setWidget(m_pAnimationList, MODIFY_MOVE, QPoint(5, 5), MODIFY_DIRECTION_WIDTH);
        m_ModObj.setWidget(m_pAnimationTable, MODIFY_RESIZE, QPoint(m_pAnimationList->width() + 10, m_pAddAnimationButton->height() + 10));
        m_ModObj.setWidget(m_pAddAnimationButton, MODIFY_MOVE, QPoint(5,5), MODIFY_DIRECTION_HEIGHT);
        m_ModObj.setWidget(m_pRemoveAnimationButton, MODIFY_MOVE, QPoint(5,5), MODIFY_DIRECTION_HEIGHT);

        m_pAddAnimationButton->setToolTip("Add new animation slot at the end.");
        m_pRemoveAnimationButton->setToolTip("Removes the last animation slot.");
    }

    void connectWidgets()
    {
        connect(m_pAddAnimationButton, SIGNAL(clicked()), this, SLOT(_onAddAnimationButtonClicked()));
        connect(m_pRemoveAnimationButton, SIGNAL(clicked()), this, SLOT(_onRemoveAnimationButtonClicked()));
        connect(m_pSpeedModifyer, SIGNAL(editingFinished()), this, SLOT(_animationSpeedChanged()));
        connect(m_pScript, SIGNAL(editingFinished()), this, SLOT(_onItemChanged()));
        connect(m_pAnimationTable, SIGNAL(onItemAdd(AnimationViewerWidget*)), this, SLOT(_onItemAdded(AnimationViewerWidget*)));
        DatabaseWidget::connectWidgets();
    }

    void setFocus()
    {
        m_pAnimationList->fillWithPrototypes();
    }

    void setAdditionalDB(DATABASE::ConstSpriteDatabaseChangerPtr pSpriteDB, DATABASE::ConstAnimationDatabaseChangerPtr pAnimationDB,
        DATABASE::ConstObjectAnimationTypeDatabaseChangerPtr pObjAniTypeDB)
    {
        m_pAnimationDB = pAnimationDB;
        m_pSpriteDB = pSpriteDB;

        m_pAnimationList->setSpriteDB(m_pSpriteDB);
        m_pAnimationList->setDB(m_pAnimationDB);
        m_pAnimationTable->setAdditionalDBs(m_pSpriteDB, m_pAnimationDB, pObjAniTypeDB);
    }

    inline DATABASE::ConstSpriteDatabaseChangerPtr getSpriteDB() const { return m_pSpriteDB; }
    inline DATABASE::ConstAnimationDatabaseChangerPtr getAnimationDB() const { return m_pAnimationDB; }

    inline QRect getBoundingRect()
    {
        return QRect(m_pBoundingLeft->value(), m_pBoundingUp->value(), m_pBoundingRight->value(), m_pBoundingBottom->value());
    }

    bool getItemFromWidgets(DATABASE::WorldObjectPrototypePtr &proto)
    {
        if (!DatabaseWidget::getItemFromWidgets(proto))
            return false;

        proto->setAnimationSpeed(m_pSpeedModifyer->value());
        proto->setBoundingRect(UInt32Rect(UInt32Point(m_pBoundingLeft->value(), m_pBoundingUp->value()),
            UInt32Point(m_pBoundingRight->value(), m_pBoundingBottom->value())));
        for (uint32 i = 1; i <= m_pAnimationTable->getCellCount(); ++i)
            proto->setAnimationInfo(i, m_pAnimationTable->getCellAnimationInfo(i-1));
        return true;
    }

    bool setWidgetsFromPrototype(const DATABASE::WorldObjectPrototypePtr &proto)
    {
        if (!DatabaseWidget::setWidgetsFromPrototype(proto))
            return false;

        disconnect(m_pBoundingLeft, SIGNAL(valueChanged(int)), this, SLOT(_boundingRectChanged(int)));
        disconnect(m_pBoundingUp, SIGNAL(valueChanged(int)), this, SLOT(_boundingRectChanged(int)));
        disconnect(m_pBoundingRight, SIGNAL(valueChanged(int)), this, SLOT(_boundingRectChanged(int)));
        disconnect(m_pBoundingBottom, SIGNAL(valueChanged(int)), this, SLOT(_boundingRectChanged(int)));
        // add animations first
        for (uint32 i = 1; i <= proto->getAnimationCount(); ++i)
            m_pAnimationTable->setCellAnimationInfo(i-1, proto->getAnimationInfo(i));
        QRect boundingRect(proto->getBoundingRect().getPositionX(), proto->getBoundingRect().getPositionY(), proto->getBoundingRect().getWidth(), proto->getBoundingRect().getHeight());
        m_pBoundingLeft->setValue(boundingRect.x());
        m_pBoundingUp->setValue(boundingRect.y());
        m_pBoundingRight->setValue(boundingRect.width());
        m_pBoundingBottom->setValue(boundingRect.height());
        m_pAnimationTable->setSceneBoundingRect(boundingRect);
        m_pSpeedModifyer->setValue(proto->getAnimationSpeed());
        updateAnimationSpeed();
        connect(m_pBoundingLeft, SIGNAL(valueChanged(int)), this, SLOT(_boundingRectChanged(int)));
        connect(m_pBoundingUp, SIGNAL(valueChanged(int)), this, SLOT(_boundingRectChanged(int)));
        connect(m_pBoundingRight, SIGNAL(valueChanged(int)), this, SLOT(_boundingRectChanged(int)));
        connect(m_pBoundingBottom, SIGNAL(valueChanged(int)), this, SLOT(_boundingRectChanged(int)));
        return true;
    }

private:
    DATABASE::ConstSpriteDatabaseChangerPtr m_pSpriteDB;
    DATABASE::ConstAnimationDatabaseChangerPtr m_pAnimationDB;
};
#endif