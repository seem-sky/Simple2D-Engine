#include "DatabaseModuleTileSet.h"
#include "moc_DatabaseModuleTileSet.h"
#include <math_extensions.h>

const uint32 MIN_COLUMN_COUNT = 1;
const uint32 MIN_ROW_COUNT = 1;

using namespace DATABASE::PROTOTYPE::TILE_SET;

DatabaseModuleTileSet::DatabaseModuleTileSet(QWidget* pParent) : QWidget(pParent), Ui_DatabaseModuleTileSet(), m_pTileDB(nullptr)
{
    setupUi(this);

    // setup table
    m_pTileSetTable->setSelectionMode(QAbstractItemView::NoSelection);

    resizeTileSetTableColumns(DEFAULT_COLUMN_COUNT);
    resizeTileSetTableRows(DEFAULT_ROW_COUNT);

    // connections
    connect(m_pHorizontalAdd, SIGNAL(clicked()), this, SLOT(_onHorizontalAddClicked()));
    connect(m_pHorizontalRemove, SIGNAL(clicked()), this, SLOT(_onHorizontalRemoveClicked()));
    connect(m_pVerticalAdd, SIGNAL(clicked()), this, SLOT(_onVerticalAddClicked()));
    connect(m_pVerticalRemove, SIGNAL(clicked()), this, SLOT(_onVerticalRemoveClicked()));
}

void DatabaseModuleTileSet::clear()
{
    m_pTileSetTable->setRowCount(0);
    m_pTileSetTable->setColumnCount(0);
    resizeTileSetTableColumns(DEFAULT_COLUMN_COUNT);
    resizeTileSetTableRows(DEFAULT_ROW_COUNT);
}

void DatabaseModuleTileSet::resizeTileSetTableColumns(uint32 columns)
{
    int32 oldColumnCount = m_pTileSetTable->columnCount();
    if (oldColumnCount == columns)
        return;
    m_pTileSetTable->setColumnCount(columns);
    if (m_pTileSetTable->columnCount() > oldColumnCount)
        _fillCells(0, oldColumnCount);
}

void DatabaseModuleTileSet::resizeTileSetTableRows(uint32 rows)
{
    int32 oldRowCount = m_pTileSetTable->rowCount();
    if (oldRowCount == rows)
        return;
    m_pTileSetTable->setRowCount(rows);
    if (m_pTileSetTable->rowCount() > oldRowCount)
        _fillCells(oldRowCount, 0);
}

void DatabaseModuleTileSet::_onHorizontalAddClicked()
{
    resizeTileSetTableColumns(m_pTileSetTable->columnCount()+1);
    //updateItem();
}

void DatabaseModuleTileSet::_onHorizontalRemoveClicked()
{
    if (m_pTileSetTable->columnCount() > MIN_COLUMN_COUNT)
    {
        resizeTileSetTableColumns(m_pTileSetTable->columnCount()-1);
        //updateItem();
    }
}

void DatabaseModuleTileSet::_onVerticalAddClicked()
{
    resizeTileSetTableRows(m_pTileSetTable->rowCount()+1);
    //updateItem();
}

void DatabaseModuleTileSet::_onVerticalRemoveClicked()
{
    if (m_pTileSetTable->rowCount() > MIN_ROW_COUNT)
    {
        resizeTileSetTableRows(m_pTileSetTable->rowCount()-1);
        //updateItem();
    }
}

void DatabaseModuleTileSet::_fillCells(int32 startRow, int32 startColumn)
{
    for (int32 row = startRow; row < m_pTileSetTable->rowCount(); ++row)
    {
        for (int32 column = startColumn; column < m_pTileSetTable->columnCount(); ++column)
            m_pTileSetTable->setCellWidget(row, column, _getNewTileDropLabel());
    }
}

TileDropLabel* DatabaseModuleTileSet::_getNewTileDropLabel() const
{
    TileDropLabel* pLabel = new TileDropLabel(m_pTileSetTable);
    pLabel->setTileDatabase(m_pTileDB);
    //connect(pLabel, SIGNAL(onDrop(uint32, Int32Point)), this, SLOT(_onTileDrop(uint32, Int32Point)));
    return pLabel;
}

void DatabaseModuleTileSet::setTileDatabase(const DATABASE::TileDatabase* pTileDB)
{
    m_pTileDB = pTileDB;
    // update cell widgets
    for (int row = 0; row < m_pTileSetTable->rowCount(); ++row)
    {
        for (int column = 0; column < m_pTileSetTable->columnCount(); ++column)
        {
            if (TileDropLabel* pLabel = dynamic_cast<TileDropLabel*>(m_pTileSetTable->cellWidget(row, column)))
                pLabel->setTileDatabase(m_pTileDB);
        }
    }
}

void DatabaseModuleTileSet::setTileID(int row, int column, uint32 ID)
{
    if (auto pLabel = dynamic_cast<TileDropLabel*>(m_pTileSetTable->cellWidget(row, column)))
        pLabel->setCurrentTileID(ID);
}

uint32 DatabaseModuleTileSet::getTileID(int row, int column) const
{
    if (auto pLabel = dynamic_cast<TileDropLabel*>(m_pTileSetTable->cellWidget(row, column)))
        return pLabel->getCurrentTileID();
    return MATH::maximum<uint32>();
}
