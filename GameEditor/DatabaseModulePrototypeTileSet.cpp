#include "DatabaseModulePrototypeTileSet.h"
#include "moc_DatabaseModulePrototypeTileSet.h"
#include "DatabaseModulePrototypeBase.h"
#include <math.h>
#include "TileDropWidget.h"

using namespace database;
using namespace ui::module;

const int32 MIN_COLUMN_COUNT = 1;
const int32 MIN_ROW_COUNT = 1;
const int32 MAX_ROW_COUNT = 50;
const int32 MAX_COLUMN_COUNT = 50;
const int32 DEFAULT_ROW_COUNT = 2;
const int32 DEFAULT_COLUMN_COUNT = 2;

PrototypeTileSet::PrototypeTileSet(const helper::CreatorInterface& creator, QWidget* pParent)
    : PrototypeInterface(pParent), Ui_DatabaseModulePrototypeTileSet(), m_Creator(creator)
{
    setupUi(this);
    clear();

    m_pTable->setFocusPolicy(Qt::NoFocus);

    // connections
    connect(m_pHorizontalAdd, SIGNAL(clicked()), this, SLOT(_onHorizontalAddClicked()));
    connect(m_pHorizontalRemove, SIGNAL(clicked()), this, SLOT(_onHorizontalRemoveClicked()));
    connect(m_pVerticalAdd, SIGNAL(clicked()), this, SLOT(_onVerticalAddClicked()));
    connect(m_pVerticalRemove, SIGNAL(clicked()), this, SLOT(_onVerticalRemoveClicked()));
}

void PrototypeTileSet::setup(const prototype::TileSet* pPrototype)
{
    m_pModuleBase->setup(pPrototype);

    setSize(pPrototype->getSize());
    for (uint32 row = 0; row < pPrototype->getSize().getHeight(); ++row)
    {
        for (uint32 column = 0; column < pPrototype->getSize().getWidth(); ++column)
            setTileID(row, column, pPrototype->getTileID(GEOMETRY::Point<uint32>(column, row)));
    }
}

void PrototypeTileSet::setupFrom(prototype::TileSet* pPrototype) const
{
    m_pModuleBase->setupFrom(pPrototype);

    pPrototype->resize(getSize());
    for (uint32 row = 0; row < pPrototype->getSize().getHeight(); ++row)
    {
        for (uint32 column = 0; column < pPrototype->getSize().getWidth(); ++column)
            pPrototype->setTileID(GEOMETRY::Point<uint32>(column, row), getTileID(row, column));
    }
}

uint32 PrototypeTileSet::getTileID(uint32 row, uint32 column) const
{
    if (auto pWidget = dynamic_cast<TileDropWidget*>(m_pTable->cellWidget(row, column)))
        return pWidget->getTileID();
    return 0;
}

void PrototypeTileSet::setTileID(uint32 row, uint32 column, uint32 ID)
{
    if (auto pWidget = dynamic_cast<TileDropWidget*>(m_pTable->cellWidget(row, column)))
        pWidget->setTileID(ID);
}

void PrototypeTileSet::clear()
{
    m_pModuleBase->clear();
    m_pTable->setColumnCount(0);
    m_pTable->setRowCount(0);
    setSize(GEOMETRY::Size<uint32>(DEFAULT_COLUMN_COUNT, DEFAULT_ROW_COUNT));
}

void setupCell(QTableWidget* pTable, const helper::CreatorInterface& creator, int32 row, int32 column)
{
    if (column >= pTable->columnCount() || row >= pTable->rowCount())
        return;

    pTable->setCellWidget(row, column, new TileDropWidget(creator, pTable));
}

void resizeColumns(QTableWidget* pTable, const helper::CreatorInterface& creator, int32 size)
{
    auto oldColumnCount = pTable->columnCount();
    size = std::max(size, MIN_COLUMN_COUNT);
    size = std::min(size, MAX_COLUMN_COUNT);
    if (oldColumnCount == size)
        return;

    pTable->setColumnCount(size);
    for (int32 column = oldColumnCount; column < size; ++column)
    {
        for (int32 row = 0; row < pTable->rowCount(); ++row)
            setupCell(pTable, creator, row, column);
    }
}

void resizeRows(QTableWidget* pTable, const helper::CreatorInterface& creator, int32 size)
{
    auto oldRowCount = pTable->rowCount();
    size = std::max(size, MIN_ROW_COUNT);
    size = std::min(size, MAX_ROW_COUNT);
    if (oldRowCount == size)
        return;

    pTable->setRowCount(size);
    for (int32 row = oldRowCount; row < size; ++row)
    {
        for (int32 column = 0; column < pTable->columnCount(); ++column)
            setupCell(pTable, creator, row, column);
    }
}

void PrototypeTileSet::setSize(const GEOMETRY::Size<uint32>& size)
{
    resizeRows(m_pTable, m_Creator, size.getHeight());
    resizeColumns(m_pTable, m_Creator, size.getWidth());
}

GEOMETRY::Size<uint32> PrototypeTileSet::getSize() const
{
    return GEOMETRY::Size<uint32>(getWidth(), getHeight());
}

void PrototypeTileSet::setWidth(uint32 width)
{
    resizeColumns(m_pTable, m_Creator, width);
}

void PrototypeTileSet::setHeight(uint32 height)
{
    resizeRows(m_pTable, m_Creator, height);
}

uint32 PrototypeTileSet::getHeight() const
{
    return m_pTable->rowCount();
}

uint32 PrototypeTileSet::getWidth() const
{
    return m_pTable->columnCount();
}

// slots
void PrototypeTileSet::_onHorizontalAddClicked()
{
    setWidth(getWidth() + 1);
}

void PrototypeTileSet::_onHorizontalRemoveClicked()
{
    if (getWidth())
        setWidth(getWidth() - 1);
}

void PrototypeTileSet::_onVerticalAddClicked()
{
    setHeight(getHeight() + 1);
}

void PrototypeTileSet::_onVerticalRemoveClicked()
{
    if (getHeight())
        setHeight(getHeight() - 1);
}
