#ifndef MAP_EDITOR_MODULE_TILES_H
#define MAP_EDITOR_MODULE_TILES_H

#include <QtWidgets/QTableWidget>
#include "AbstractPrototypeTable.h"

class MapEditorModuleTiles : public AbstractPrototypeTable
{
private:
    void _setup();
};

class MapEditorModuleAutoTiles : public AbstractPrototypeTable
{
private:
    void _setup();
};

#endif
