#include "MapEditorModuleTiles.h"
#include "QtGlobal.h"
#include "Config.h"

void MapEditorModuleTiles::_setup()
{
    clear();
    if (!m_pDBMgr)
        return;

    auto pTileDB = m_pDBMgr->getTileDatabase();
    if (!pTileDB)
        return;

    const uint32 tilesPerRow = width()/(TILE_SIZE+2);
    setColumnCount(tilesPerRow);
    setRowCount(std::ceil(double(pTileDB->getSize()+1)/tilesPerRow));

    for (uint32 i = 0; i <= pTileDB->getSize(); ++i)
    {
        auto pTileFrame = new TexturePrototypeFrameEx(this);
        if (auto pPrototype = pTileDB->getOriginalPrototype(i))
            pTileFrame->setPrototype(pPrototype);
        setCellWidget(i/tilesPerRow, i%tilesPerRow, pTileFrame);
    }
}

void MapEditorModuleAutoTiles::_setup()
{
    clear();
    if (!m_pDBMgr)
        return;

    auto pTileDB = m_pDBMgr->getTileDatabase();
    auto pAutoTileDB = m_pDBMgr->getAutoTileDatabase();

    if (!pAutoTileDB || !pTileDB)
        return;
    const uint32 tilesPerRow = width()/(TILE_SIZE+2);
    setColumnCount(tilesPerRow);
    setRowCount(std::ceil(double(pAutoTileDB->getSize())/tilesPerRow));
    for (uint32 i = 0; i < pAutoTileDB->getSize(); ++i)
    {
        auto pTileFrame = new TexturePrototypeFrameEx(this);
        if (auto pAutoTilePrototype = pAutoTileDB->getOriginalPrototype(i+1))
        {
            if (auto pPrototype = pTileDB->getOriginalPrototype(pAutoTilePrototype->getTileID(DATABASE::AUTO_TILE::INDEX_INNER_CENTER)))
                pTileFrame->setPrototype(pPrototype);
        }
        setCellWidget(i/tilesPerRow, i%tilesPerRow, pTileFrame);
    }
}
