#ifndef DATABASE_MODULE_TILE_SETS_H
#define DATABASE_MODULE_TILE_SETS_H

#include "DatabaseModuleDragDrop.h"

class MapEditorModuleTileSets : public DatabaseModuleDragList
{
private:
    QWidget* _setupTooltipWidget(uint32 uiPrototypeID);

public:
    MapEditorModuleTileSets(QWidget *pParent = nullptr);

    void setTileDatabase(const DATABASE::TileDatabase *pDB);

    void setModel(IDatabaseModel *pModel);

private:
    const DATABASE::TileDatabase *m_pTileDB;
};

#endif
