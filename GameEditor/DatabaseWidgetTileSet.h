#ifndef DATABASE_WIDGET_TILE_SET_H
#define DATABASE_WIDGET_TILE_SET_H

#include "DatabaseWidgetBase.h"
#include "DatabaseModuleDragDrop.h"
#include "DatabaseModuleTileSet.h"

class DatabaseWidgetTileSet : public DatabaseWidgetBase
{
protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::PROTOTYPE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::PROTOTYPE::Prototype* pPrototype);
    virtual void clear();

public:
    DatabaseWidgetTileSet(QWidget* pParent = nullptr);

    void setDatabaseMgr(DATABASE::DatabaseMgr& DBMgr);
    void setTileDatabaseModel(DATABASE::ConstDatabaseModel* pModel);

private:
    DatabaseModuleTileSet* m_pModuleTileSet;
    DatabaseModuleTooltipList* m_pModuleTileList;
};

#endif
