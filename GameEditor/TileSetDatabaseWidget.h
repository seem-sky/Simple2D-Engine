#ifndef TILE_SET_DATABASE_WIDGET_H
#define TILE_SET_DATABASE_WIDGET_H

#include "DatabaseWidget.h"

const uint32 MIN_COLUMN_COUNT = 1;
const uint32 MIN_ROW_COUNT = 1;

class TileSetDatabaseWidget : public DatabaseWidget<DATABASE::TILE_SET::TileSetPrototype>, Ui_TileSetDatabaseWidget
{
    Q_OBJECT
private:
    void _fillCells(int32 startRow, int32 startColumn);
    TileDropLabel* _getNewTileDropLabel();

protected:
    bool getItemFromWidgets(DATABASE::TileSetPrototypePtr &proto);
    bool setWidgetsFromPrototype(const DATABASE::TileSetPrototypePtr &proto);
    void clearWidgets();

public:
    TileSetDatabaseWidget(QWidget *pParent = NULL);
    void setTileDB(DATABASE::ConstTileDatabaseChangerPtr pDB);
    void setFocus();

    void resizeTileSetTableColumns(uint32 columns);
    void resizeTileSetTableRows(uint32 rows);

private slots:
    void _onHorizontalAddClicked(bool clicked);
    void _onHorizontalRemoveClicked(bool clicked);
    void _onVerticalAddClicked(bool clicked);
    void _onVerticalRemoveClicked(bool clicked);

    void _onTileDrop(uint32 uiID, Int32Point pos);

    DATABASE_WIDGET_SLOTS

private:
    DATABASE::ConstTileDatabaseChangerPtr m_pTileDB;
};
#endif