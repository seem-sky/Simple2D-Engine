#ifndef DATABASE_WIDGET_WORLD_OBJECT_H
#define DATABASE_WIDGET_WORLD_OBJECT_H

#include "DatabaseWidgetBase.h"
#include <QtWidgets/QComboBox>
#include "DatabaseModuleWorldObjectAnimation.h"
#include "DeactivatedTabWidget.h"

class DatabaseWidgetWorldObject : public DatabaseWidgetBase
{
    Q_OBJECT
protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::PROTOTYPE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::PROTOTYPE::Prototype* pPrototype);
    virtual void clear();

public:
    DatabaseWidgetWorldObject(QWidget* pParent = nullptr);

    void setDatabaseMgr(DATABASE::DatabaseMgr& DBMgr);
    void setSpriteDatabaseModel(DATABASE::ConstDatabaseModel* pModel);
    void setAnimationDatabaseModel(DATABASE::ConstDatabaseModel* pModel);

private:
    // tab widget
    DeactivatedTabWidget* m_pModuleTab;

    DatabaseModuleWorldObjectAnimation *m_pModuleAnimation;
};

#endif
