#include "MapEditorModuleTiles.h"
#include "QtGlobal.h"
#include "Config.h"
#include "AutoTileCache.h"
#include "TileCache.h"
#include <QtGui/QPainter>

/*#####
# tile module
#####*/
TilePixmapWidget::TilePixmapWidget(uint32 ID, QWidget* pParent) : AbstractPixmapWidget(ID, pParent)
{
}

void TilePixmapWidget::drawPixmap()
{
    if (auto pPixmap = GTileCache::get()->getItem(getID()))
    {
        QPainter painter(this);
        painter.drawPixmap(0, 0,* pPixmap);
    }
}

MapEditorModuleTiles::MapEditorModuleTiles(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : AbstractPrototypeTable(DBMgr, pParent)
{
}

void MapEditorModuleTiles::_setup()
{
    clear();

    auto pTileDB = m_DBMgr.getTileDatabase();
    if (!pTileDB)
        return;

    const uint32 tilesPerRow = width()/(TILE_SIZE+2);
    setColumnCount(tilesPerRow);
    setRowCount(std::ceil(double(pTileDB->getSize()+1)/tilesPerRow));

    for (uint32 i = 0; i <= pTileDB->getSize(); ++i)
    {
        auto pItem = new TilePixmapWidget(i, this);
        // click first item, so border will be visible
        if (i == 0)
        {
            clickItem(BRUSH::BrushIndex::BRUSH_LEFT, pItem);
            clickItem(BRUSH::BrushIndex::BRUSH_RIGHT, pItem);
        }
        setCellWidget(i/tilesPerRow, i%tilesPerRow, pItem);
    }
}

/*#####
# auto tile module
#####*/
AutoTilePixmapWidget::AutoTilePixmapWidget(uint32 ID, QWidget* pParent) : AbstractPixmapWidget(ID, pParent)
{
}

void AutoTilePixmapWidget::drawPixmap()
{
    if (auto pAutoTile = GAutoTileCache::get()->getItem(getID()))
    {
        if (auto pPixmap = pAutoTile->getPixmap(DATABASE::AUTO_TILE::INDEX_INNER_CENTER))
        {
            QPainter painter(this);
            painter.drawPixmap(0, 0,* pPixmap);
        }
    }
}

MapEditorModuleAutoTiles::MapEditorModuleAutoTiles(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : AbstractPrototypeTable(DBMgr, pParent)
{
}


void MapEditorModuleAutoTiles::_setup()
{
    clear();

    auto pTileDB = m_DBMgr.getTileDatabase();
    auto pAutoTileDB = m_DBMgr.getAutoTileDatabase();

    if (!pAutoTileDB || !pTileDB)
        return;
    const uint32 tilesPerRow = width()/(TILE_SIZE+2);
    setColumnCount(tilesPerRow);
    setRowCount(std::ceil(double(pAutoTileDB->getSize())/tilesPerRow));
    for (uint32 i = 0; i < pAutoTileDB->getSize(); ++i)
        setCellWidget(i/tilesPerRow, i%tilesPerRow, new AutoTilePixmapWidget(i+1, this));
}
