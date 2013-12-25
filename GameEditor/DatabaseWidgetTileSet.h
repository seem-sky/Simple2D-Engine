#ifndef DATABASE_WIDGET_TILE_SET_H
#define DATABASE_WIDGET_TILE_SET_H

#include "DatabaseWidgetBase.h"
#include "DatabaseModuleDragDrop.h"
#include "DatabaseModuleTileSet.h"

class DatabaseWidgetTileSet : public DatabaseWidgetBase
{
protected:
    virtual void setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype);
    virtual void setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype);
    virtual void clear();

public:
    DatabaseWidgetTileSet(const DATABASE::TileSetDatabase* pDatabase = nullptr, QWidget* pParent = nullptr);

    void setTileDatabaseModel(DatabaseModel<DATABASE::TileDatabase>* pDBModel);

private:
    DatabaseModuleTileSet* m_pModuleTileSet;
    DatabaseModuleDragList* m_pModuleTileList;
};

#endif
