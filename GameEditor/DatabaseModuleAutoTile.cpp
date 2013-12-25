#include "DatabaseModuleAutoTile.h"

using namespace DATABASE::AUTO_TILE;

DatabaseModuleAutoTile::DatabaseModuleAutoTile(QWidget* pParent) : QWidget(pParent), Ui_DatabaseModuleAutoTile()
{
    setupUi(this);
}

AUTO_TILE_INDEX DatabaseModuleAutoTile::_getLabelIndex(const TileDropLabel* pLabel) const
{
    if (pLabel)
    {
        for (uint32 i = 0; i < INDEX_NONE; ++i)
        {
            if (pLabel == _getLabel(static_cast<AUTO_TILE_INDEX>(i)))
                return static_cast<AUTO_TILE_INDEX>(i);
        }
    }
    return INDEX_NONE;
}

TileDropLabel* DatabaseModuleAutoTile::_getLabel(AUTO_TILE_INDEX index) const
{
    switch (index)
    {
    case INDEX_TOP_LEFT: return m_pTopLeft;
    case INDEX_TOP: return m_pTopCenter;
    case INDEX_TOP_RIGHT: return m_pTopRight;
    case INDEX_LEFT: return m_pCenterLeft;
    case INDEX_CENTER: return m_pCenter;
    case INDEX_RIGHT: return m_pCenterRight;
    case INDEX_BOTTOM_LEFT: return m_pBottomLeft;
    case INDEX_BOTTOM: return m_pBottomCenter;
    case INDEX_BOTTOM_RIGHT: return m_pBottomRight;
    case INDEX_INNER_CENTER: return m_pInnerEdges;
    }
    return nullptr;
}

TileArray DatabaseModuleAutoTile::getTiles() const
{
    TileArray tiles;
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel* pLabel = _getLabel(static_cast<AUTO_TILE_INDEX>(i)))
            tiles.at(i) = pLabel->getCurrentTileID();
    }
    return tiles;
}

void DatabaseModuleAutoTile::setTiles(const DATABASE::AUTO_TILE::TileArray& tiles)
{
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel* pLabel = _getLabel(static_cast<AUTO_TILE_INDEX>(i)))
           pLabel->setCurrentTileID(tiles.at(i));
    }
}

void DatabaseModuleAutoTile::setTileDatabase(const DATABASE::TileDatabase* pTileDB)
{
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel* pLabel = _getLabel(static_cast<AUTO_TILE_INDEX>(i)))
            pLabel->setTileDatabase(pTileDB);
    }
}

void DatabaseModuleAutoTile::clear()
{
    for (uint32 i = 0; i < AUTO_TILE_SET_COUNT; ++i)
    {
        if (TileDropLabel* pLabel = _getLabel(static_cast<AUTO_TILE_INDEX>(i)))
            pLabel->setCurrentTileID(0);
    }
}
