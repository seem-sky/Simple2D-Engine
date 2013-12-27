#ifndef DATABASE_WIDGET_AUTO_TILE_H
#define DATABASE_WIDGET_AUTO_TILE_H

#include "DatabaseWidgetBase.h"
#include "DatabaseModuleAutoTile.h"
#include "DatabaseModuleDragDrop.h"

class DatabaseWidgetAutoTile : public DatabaseWidgetBase
{
protected:
    void setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype);
    void setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype);

public:
    DatabaseWidgetAutoTile(QWidget* pParent = nullptr);

    void clear();

    void setTileDatabaseModel(DATABASE::ConstDatabaseModel* pTileDBModel);

private:
    DatabaseModuleAutoTile* m_pModuleAutoTile;
    DatabaseModuleTextureDragList* m_pModuleTileList;
};

#endif
