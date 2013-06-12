#ifndef WORLD_OBJECT_DATABASE_WINDOW_H
#define WORLD_OBJECT_DATABASE_WINDOW_H

#include "DatabaseWidget.h"
#include "Ui/UI_WorldObjectDatabaseWidget.h"

/*#####
# WorldObjectDatabaseWidget
#####*/
class WorldObjectDatabaseWidget : public DatabaseWidget<DATABASE::WorldObjectPrototype>, Ui_WorldObjectDatabaseWidget
{
    Q_OBJECT
private:
    void _fillAnimationList();

protected:
    QRect getBoundingRect();
    void updateAnimationSpeed(AnimationView* pViewer, const QRect &uiAnimationSpeed);
    void updateAnimationScene(AnimationView* pViewer, const QRect &boundingRect);
    void setupAnimationViewer(AnimationView* pViewer, uint32 uiAnimationID);
    bool getItemFromWidgets(DATABASE::WorldObjectPrototypePtr &proto);
    bool setWidgetsFromPrototype(const DATABASE::WorldObjectPrototypePtr &proto);

public:
    WorldObjectDatabaseWidget(QWidget *pParent = NULL);

    void setFocus();

    void setAdditionalDB(DATABASE::ConstSpriteDatabaseChangerPtr pSpriteDB, DATABASE::ConstAnimationDatabaseChangerPtr pAnimationDB,
        DATABASE::ConstObjectAnimationTypeDatabaseChangerPtr pObjAniTypeDB);
    DATABASE::ConstSpriteDatabaseChangerPtr getSpriteDB() const { return m_pSpriteDB; }
    DATABASE::ConstAnimationDatabaseChangerPtr getAnimationDB() const { return m_pAnimationDB; }

private slots:
    void _onRemoveAnimationButtonClicked();
    void _onAddAnimationButtonClicked();
    void _updateAnimationSpeed();
    void _dataChanged(int value);
    void _onDrop(AnimationView *pTarget, uint32 uiID, Point<int32> pos);
    void _onItemChanged();
    void _onItemAdded(AnimationViewerWidget *pWidget);
    void _onItemRemoved(AnimationViewerWidget *pWidget);

    //parent slots
    void _resizeButtonClicked() { resizeDatabase(getListCountValue()); }
    void _currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) { currentItemChanged(current); }
    void _changeName() { changeName(); }

private:
    DATABASE::ConstSpriteDatabaseChangerPtr m_pSpriteDB;
    DATABASE::ConstAnimationDatabaseChangerPtr m_pAnimationDB;
};
#endif