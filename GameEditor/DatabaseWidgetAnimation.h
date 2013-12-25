#ifndef DATABASE_WIDGET_ANIMATION_H
#define DATABASE_WIDGET_ANIMATION_H

#include "DatabaseWidgetBase.h"
#include "DatabaseModuleAnimation.h"
#include "DatabaseModuleDragDrop.h"

class DatabaseWidgetAnimation : public DatabaseWidgetBase 
{
protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype);
    virtual void clear();

public:
    DatabaseWidgetAnimation(QWidget* pParent = nullptr);

    void setSpriteDatabaseModel(SpriteDatabaseModel* pModel);

protected:
    DatabaseModuleAnimation* m_pModuleAnimation;
    DatabaseModuleTextureDragList* m_pModuleSpriteList;
};

#endif
