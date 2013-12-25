#ifndef DATABASE_MODULE_TILE_SET_H
#define DATABASE_MODULE_TILE_SET_H

#include "UI/UI_DatabaseModuleTileSet.h"
#include "DatabaseModuleDragDrop.h"

class DatabaseModuleTileSet : public QWidget, Ui_DatabaseModuleTileSet
{
    Q_OBJECT
private:
    void _fillCells(int32 startRow, int32 startColumn);
    TileDropLabel* _getNewTileDropLabel() const;

public:
    DatabaseModuleTileSet(QWidget* pParent = nullptr);

    void resizeTileSetTableColumns(uint32 columns);
    void resizeTileSetTableRows(uint32 rows);
    inline uint32 getTileSetTableColumnCount() const { return m_pTileSetTable->columnCount(); }
    inline uint32 getTileSetTableRowCount() const { return m_pTileSetTable->rowCount(); }

    void setTileID(int row, int column, uint32 ID);
    uint32 getTileID(int row, int column) const;

    void clear();

    void setTileDatabase(const DATABASE::TileDatabase* pTileDB);

private slots:
    void _onHorizontalAddClicked();
    void _onHorizontalRemoveClicked();
    void _onVerticalAddClicked();
    void _onVerticalRemoveClicked();

private:
    const DATABASE::TileDatabase* m_pTileDB;
};

#endif
