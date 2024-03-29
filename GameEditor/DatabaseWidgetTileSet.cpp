#include "DatabaseWidgetTileSet.h"

using namespace DATABASE;
using namespace PROTOTYPE;

DatabaseWidgetTileSet::DatabaseWidgetTileSet(QWidget* pParent) : DatabaseWidgetBase(pParent),
    m_pModuleTileSet(new DatabaseModuleTileSet(this)), m_pModuleTileList(new DatabaseModuleTextureDragList(this))
{
    // setup tileset module
    if (QGridLayout* pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->addWidget(m_pModuleTileSet, 0, 1);

        // add tile list module
        QVBoxLayout* pBoxLayout(new QVBoxLayout());
        pBoxLayout->setSpacing(6);

        // setup "tiles:" label
        QLabel* pLabel(new QLabel("tiles:"));
        QFont font;
        font.setPointSize(8);
        font.setBold(true);
        font.setWeight(75);
        pLabel->setFont(font);
        pBoxLayout->addWidget(pLabel);

        pBoxLayout->addWidget(m_pModuleTileList);
        pLayout->addLayout(pBoxLayout, 0, 2);
    }
}

void DatabaseWidgetTileSet::setDatabaseMgr(DatabaseMgr& DBMgr)
{
    m_pModuleList->setDatabaseModel(new DatabaseModel(DBMgr, DatabaseType::TILE_SET_DATABASE));
    m_pModuleTileSet->setTileDatabase(DBMgr.getTileDatabase());
}

void DatabaseWidgetTileSet::setTileDatabaseModel(ConstDatabaseModel* pModel)
{
    m_pModuleTileList->setModel(pModel);
}

void DatabaseWidgetTileSet::setupWidgetsFromPrototype(const Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<const TILE_SET::TileSetPrototype*>(pPrototype))
    {
        auto size = pProto->getTileSetSize();
        m_pModuleTileSet->resizeTileSetTableColumns(size.getX());
        m_pModuleTileSet->resizeTileSetTableRows(size.getY());
        for (uint32 row = 0; row < size.getY(); ++row)
        {
            for (uint32 column = 0; column < size.getX(); ++column)
                m_pModuleTileSet->setTileID(row, column, pProto->getTileID(GEOMETRY::Point<uint32>(column, row)));
        }
    }
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetTileSet::setupPrototypeFromWidgets(Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<TILE_SET::TileSetPrototype*>(pPrototype))
    {
        GEOMETRY::Point<uint32> size(m_pModuleTileSet->getTileSetTableColumnCount(), m_pModuleTileSet->getTileSetTableRowCount());
        pProto->resizeTileSet(size);
        for (uint32 row = 0; row < size.getY(); ++row)
        {
            for (uint32 column = 0; column < size.getX(); ++column)
                pProto->setTileID(GEOMETRY::Point<uint32>(column, row), m_pModuleTileSet->getTileID(row, column));
        }
    }
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetTileSet::clear()
{
    m_pModuleTileSet->clear();
    DatabaseWidgetBase::clear();
}
