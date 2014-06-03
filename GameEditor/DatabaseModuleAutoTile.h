#ifndef DATABASE_MODULE_AUTO_TILE_H
#define DATABASE_MODULE_AUTO_TILE_H

#include "UI/UI_DatabaseModuleAutoTile.h"

class DatabaseModuleAutoTile : public QWidget, Ui_DatabaseModuleAutoTile
{
private:
    DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_INDEX _getLabelIndex(const TileDropLabel* pLabel) const;
    TileDropLabel* _getLabel(DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_INDEX index) const;

public:
    DatabaseModuleAutoTile(QWidget* pParent = nullptr);

    DATABASE::PROTOTYPE::AUTO_TILE::TileArray getTiles() const;
    void setTiles(const DATABASE::PROTOTYPE::AUTO_TILE::TileArray& tiles);

    void setTileDatabase(const DATABASE::TileDatabase* pTileDB);

    void clear();
};

#endif
