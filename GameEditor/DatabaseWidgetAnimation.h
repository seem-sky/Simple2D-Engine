#ifndef DATABASE_WIDGET_ANIMATION_H
#define DATABASE_WIDGET_ANIMATION_H

#include "DatabaseWidgetBase.h"
#include "DatabaseModuleAnimation.h"
#include "DatabaseModuleDragDrop.h"

class DatabaseWidgetAnimation : public DatabaseWidgetBase 
{
protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::PROTOTYPE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::PROTOTYPE::Prototype* pPrototype);
    virtual void clear();

public:
    DatabaseWidgetAnimation(QWidget* pParent = nullptr);

    void setDatabaseMgr(DATABASE::DatabaseMgr& DBMgr);
    void setSpriteDatabaseModel(DATABASE::ConstDatabaseModel* pModel);

protected:
    DatabaseModuleAnimation* m_pModuleAnimation;
    DatabaseModuleTextureDragList* m_pModuleSpriteList;
};

#endif
