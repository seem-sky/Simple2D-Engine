#include "MapEditorModuleTiles.h"
#include "QtGlobal.h"
#include "Config.h"
#include <QtGui/QPainter>
#include <Core/Cache/Manager.h>

/*#####
# tile module
#####*/
TilePixmapWidget::TilePixmapWidget(uint32 ID, CACHE::Manager& cacheMgr, QWidget* pParent)
    : AbstractPixmapWidget(ID, pParent), m_CacheMgr(cacheMgr)
{}

void TilePixmapWidget::drawPixmap()
{
    if (!getID())
        return;
    auto info = m_CacheMgr.getTileCache().get(getID());
    if (!info.isValid())
        return;
    QPainter painter(this);
    painter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), MAP::TILE_SIZE, MAP::TILE_SIZE);
}

MapEditorModuleTiles::MapEditorModuleTiles(CACHE::Manager& cacheMgr, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent)
    : AbstractPrototypeTable(DBMgr, pParent), m_CacheMgr(cacheMgr)
{}

void MapEditorModuleTiles::_setup()
{
    clear();

    auto pTileDB = m_DBMgr.getTileDatabase();
    if (!pTileDB)
        return;

    const uint32 tilesPerRow = width() / (MAP::TILE_SIZE + 2);
    setColumnCount(tilesPerRow);
    setRowCount(std::ceil(double(pTileDB->getSize()+1)/tilesPerRow));

    for (uint32 i = 0; i <= pTileDB->getSize(); ++i)
    {
        auto pItem = new TilePixmapWidget(i, m_CacheMgr, this);
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
AutoTilePixmapWidget::AutoTilePixmapWidget(uint32 ID, CACHE::Manager& cacheMgr, QWidget* pParent)
    : AbstractPixmapWidget(ID, pParent), m_CacheMgr(cacheMgr)
{}

void AutoTilePixmapWidget::drawPixmap()
{
    auto info = m_CacheMgr.getAutoTileCache().get(getID(), DATABASE::PROTOTYPE::AUTO_TILE::INDEX_INNER_CENTER);
    if (info.isValid())
    {
        QPainter painter(this);
        painter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), MAP::TILE_SIZE, MAP::TILE_SIZE);
    }
}

MapEditorModuleAutoTiles::MapEditorModuleAutoTiles(CACHE::Manager& cacheMgr, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent)
    : AbstractPrototypeTable(DBMgr, pParent), m_CacheMgr(cacheMgr)
{}


void MapEditorModuleAutoTiles::_setup()
{
    clear();

    auto pTileDB = m_DBMgr.getTileDatabase();
    auto pAutoTileDB = m_DBMgr.getAutoTileDatabase();

    if (!pAutoTileDB || !pTileDB)
        return;
    const uint32 tilesPerRow = width() / (MAP::TILE_SIZE + 2);
    setColumnCount(tilesPerRow);
    setRowCount(std::ceil(double(pAutoTileDB->getSize())/tilesPerRow));
    for (uint32 i = 0; i < pAutoTileDB->getSize(); ++i)
        setCellWidget(i/tilesPerRow, i%tilesPerRow, new AutoTilePixmapWidget(i+1, m_CacheMgr, this));
}
