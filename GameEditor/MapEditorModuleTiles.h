#ifndef MAP_EDITOR_MODULE_TILES_H
#define MAP_EDITOR_MODULE_TILES_H

#include <QtWidgets/QTableWidget>
#include "AbstractPrototypeTable.h"

namespace CACHE
{
    class Manager;
}

/*#####
# tile module
#####*/
class TilePixmapWidget : public AbstractPixmapWidget
{
protected:
    void drawPixmap();

public:
    TilePixmapWidget(uint32 ID, CACHE::Manager& cacheMgr, QWidget* pParent = nullptr);

private:
    CACHE::Manager& m_CacheMgr;
};

class MapEditorModuleTiles : public AbstractPrototypeTable
{
private:
    void _setup();

    MAP::BRUSH::BrushInfo::Type getType() const { return MAP::BRUSH::BrushInfo::Type::TILE; }

public:
    MapEditorModuleTiles(CACHE::Manager& cacheMgr, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);

private:
    CACHE::Manager& m_CacheMgr;
};

/*#####
# auto tile module
#####*/
class AutoTilePixmapWidget : public AbstractPixmapWidget
{
protected:
    void drawPixmap();

public:
    AutoTilePixmapWidget(uint32 ID, QWidget* pParent = nullptr);
};


class MapEditorModuleAutoTiles : public AbstractPrototypeTable
{
private:
    void _setup();

    MAP::BRUSH::BrushInfo::Type getType() const { return MAP::BRUSH::BrushInfo::Type::AUTO_TILE; }

public:
    MapEditorModuleAutoTiles(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent = nullptr);
};

#endif
