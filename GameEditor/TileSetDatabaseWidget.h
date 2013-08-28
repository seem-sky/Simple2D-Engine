#ifndef TILE_SET_DATABASE_H
#define TILE_SET_DATABASE_H

#include "UI/UI_TileSetDatabaseWidget.h"
#include "DatabaseWidget.h"

class TileSetDatabaseWidget : public DatabaseWidget<DATABASE::TILE_SET::TileSetPrototype, DATABASE::TILE_SET_INDEX>, Ui_TileSetDatabaseWidget
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