#include "TileSetDatabaseWidget.h"
#include "moc_TileSetDatabaseWidget.h"

const uint32 MIN_COLUMN_COUNT = 1;
const uint32 MIN_ROW_COUNT = 1;

using namespace DATABASE;

TileSetDatabaseWidget::TileSetDatabaseWidget(QWidget *pParent) : DatabaseWidget(pParent), Ui_TileSetDatabaseWidget()
{
    Ui_TileSetDatabaseWidget::setupUi(this);
    m_ModObj.setWidget(m_pTileList, MODIFY_RESIZE, QPoint(0, 5), MODIFY_DIRECTION_HEIGHT);
    connect(m_pHorizontalAdd, SIGNAL(clicked(bool)), this, SLOT(_onHorizontalAddClicked(bool)));
    connect(m_pHorizontalRemove, SIGNAL(clicked(bool)), this, SLOT(_onHorizontalRemoveClicked(bool)));
    connect(m_pVerticalAdd, SIGNAL(clicked(bool)), this, SLOT(_onVerticalAddClicked(bool)));
    connect(m_pVerticalRemove, SIGNAL(clicked(bool)), this, SLOT(_onVerticalRemoveClicked(bool)));
    connectWidgets();
    resizeTileSetTableColumns(TILE_SET::DEFAULT_COLUMN_COUNT);
    resizeTileSetTableRows(TILE_SET::DEFAULT_ROW_COUNT);
    m_pTileList->showNULLPrototype(true);
}

TileDropLabel* TileSetDatabaseWidget::_getNewTileDropLabel()
{
    TileDropLabel *pLabel = new TileDropLabel(m_pTileSetTable);
    pLabel->setTileDB(m_pTileDB);
    connect(pLabel, SIGNAL(onDrop(uint32, Int32Point)), this, SLOT(_onTileDrop(uint32, Int32Point)));
    return pLabel;
}

void TileSetDatabaseWidget::_fillCells(int32 startRow, int32 startColumn)
{
    for (int32 row = startRow; row < m_pTileSetTable->rowCount(); ++row)
    {
        for (int32 column = startColumn; column < m_pTileSetTable->columnCount(); ++column)
        {
            m_pTileSetTable->setCellWidget(row, column, _getNewTileDropLabel());
        }
    }
}

void TileSetDatabaseWidget::resizeTileSetTableColumns(uint32 columns)
{
    int32 oldColumnCount = m_pTileSetTable->columnCount();
    if (oldColumnCount == columns)
        return;
    m_pTileSetTable->setColumnCount(columns);
    if (m_pTileSetTable->columnCount() > oldColumnCount)
        _fillCells(0, oldColumnCount);
}

void TileSetDatabaseWidget::resizeTileSetTableRows(uint32 rows)
{
    int32 oldRowCount = m_pTileSetTable->rowCount();
    if (oldRowCount == rows)
        return;
    m_pTileSetTable->setRowCount(rows);
    if (m_pTileSetTable->rowCount() > oldRowCount)
        _fillCells(0, oldRowCount);
}

void TileSetDatabaseWidget::_onHorizontalAddClicked(bool clicked)
{
    resizeTileSetTableColumns(m_pTileSetTable->columnCount()+1);
    updateItem();
}

void TileSetDatabaseWidget::_onHorizontalRemoveClicked(bool clicked)
{
    if (m_pTileSetTable->columnCount() > MIN_COLUMN_COUNT)
    {
        resizeTileSetTableColumns(m_pTileSetTable->columnCount()-1);
        updateItem();
    }
}

void TileSetDatabaseWidget::_onVerticalAddClicked(bool clicked)
{
    resizeTileSetTableRows(m_pTileSetTable->rowCount()+1);
    updateItem();
}

void TileSetDatabaseWidget::_onVerticalRemoveClicked(bool clicked)
{
    if (m_pTileSetTable->rowCount() > MIN_ROW_COUNT)
    {
        resizeTileSetTableRows(m_pTileSetTable->rowCount()-1);
        updateItem();
    }
}

void TileSetDatabaseWidget::setTileDB(DATABASE::ConstTileDatabaseChangerPtr pDB)
{
    m_pTileDB = pDB;
    m_pTileList->setDB(m_pTileDB);
    for (int32 x = 0; x < m_pTileSetTable->columnCount(); ++x)
    {
        for (int32 y = 0; y < m_pTileSetTable->rowCount(); ++y)
        {
            if (TileDropLabel *pLabel = dynamic_cast<TileDropLabel*>(m_pTileSetTable->cellWidget(y, x)))
                pLabel->setTileDB(pDB);
        }
    }
}

void TileSetDatabaseWidget::setFocus()
{
    m_pTileList->fillWithPrototypes();
    for (int32 row = 0; row < m_pTileSetTable->rowCount(); ++row)
    {
        for (int32 column = 0; column < m_pTileSetTable->columnCount(); ++column)
        {
            if (TileDropLabel *pLabel = dynamic_cast<TileDropLabel*>(m_pTileSetTable->cellWidget(row, column)))
                pLabel->drawCurrentTile();
        }
    }
}

void TileSetDatabaseWidget::_onTileDrop(uint32 uiID, Int32Point pos)
{
    updateItem();
}

void TileSetDatabaseWidget::clearWidgets()
{
    DatabaseWidget::clearWidgets();
    m_pTileSetTable->setColumnCount(0);
    m_pTileSetTable->setRowCount(0);
    resizeTileSetTableColumns(TILE_SET::DEFAULT_COLUMN_COUNT);
    resizeTileSetTableRows(TILE_SET::DEFAULT_ROW_COUNT);
}

bool TileSetDatabaseWidget::setWidgetsFromPrototype(const TileSetPrototypePtr &proto)
{
    if (!DatabaseWidget::setWidgetsFromPrototype(proto) || !m_pTileDB)
        return false;
    resizeTileSetTableColumns(proto->getTileCount().x);
    resizeTileSetTableRows(proto->getTileCount().y);
    for (int32 row = 0; row < m_pTileSetTable->rowCount(); ++row)
    {
        for (int32 column = 0; column < m_pTileSetTable->columnCount(); ++column)
        {
            TileDropLabel *pLabel = _getNewTileDropLabel();
            pLabel->setCurrentTileID(proto->getTileID(UInt32Point(column, row)));
            m_pTileSetTable->setCellWidget(row, column, pLabel);
        }
    }
    return true;
}

bool TileSetDatabaseWidget::getItemFromWidgets(TileSetPrototypePtr &proto)
{
    if (!DatabaseWidget::getItemFromWidgets(proto))
        return false;
    proto->resizeTiles(UInt32Point(m_pTileSetTable->columnCount(), m_pTileSetTable->rowCount()));
    for (int32 row = 0; row < m_pTileSetTable->rowCount(); ++row)
    {
        for (int32 column = 0; column < m_pTileSetTable->columnCount(); ++column)
        {
            if (TileDropLabel *pLabel = dynamic_cast<TileDropLabel*>(m_pTileSetTable->cellWidget(row, column)))
                proto->setTileID(UInt32Point(column, row), pLabel->getCurrentTileID());
        }
    }
    return true;
}