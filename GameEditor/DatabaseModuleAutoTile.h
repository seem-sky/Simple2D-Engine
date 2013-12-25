#ifndef DATABASE_MODULE_AUTO_TILE_H
#define DATABASE_MODULE_AUTO_TILE_H

#include "UI/UI_DatabaseModuleAutoTile.h"

class DatabaseModuleAutoTile : public QWidget, Ui_DatabaseModuleAutoTile
{
private:
    DATABASE::AUTO_TILE::AUTO_TILE_INDEX _getLabelIndex(const TileDropLabel* pLabel) const;
    TileDropLabel* _getLabel(DATABASE::AUTO_TILE::AUTO_TILE_INDEX index) const;

public:
    DatabaseModuleAutoTile(QWidget* pParent = nullptr);

    DATABASE::AUTO_TILE::TileArray getTiles() const;
    void setTiles(const DATABASE::AUTO_TILE::TileArray& tiles);

    void setTileDatabase(const DATABASE::TileDatabase* pTileDB);

    void clear();
};

#endif
