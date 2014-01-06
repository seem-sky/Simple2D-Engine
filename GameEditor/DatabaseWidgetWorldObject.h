#ifndef DATABASE_WIDGET_WORLD_OBJECT_H
#define DATABASE_WIDGET_WORLD_OBJECT_H

#include "DatabaseWidgetBase.h"
#include <QtWidgets/QComboBox>
#include "DatabaseModuleDragDrop.h"
#include "DatabaseModuleVisualViewer.h"

class DatabaseWidgetWorldObject : public DatabaseWidgetBase
{
    Q_OBJECT
protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype);
    virtual void clear();

public:
    DatabaseWidgetWorldObject(QWidget* pParent = nullptr);

    void setDatabaseMgr(DATABASE::DatabaseMgr& DBMgr);
    void setSpriteDatabaseModel(DATABASE::ConstDatabaseModel* pModel);
    void setAnimationDatabaseModel(DATABASE::ConstDatabaseModel* pModel);

private slots:
    void _onVisualTypeChanged(int);

private:
    // source lists
    DatabaseModuleTextureDragList* m_pModuleSpriteList;

    DatabaseModuleVisualViewer* m_pModuleVisualViewer;
    QComboBox* m_pVisualTypeBox;
};

#endif
