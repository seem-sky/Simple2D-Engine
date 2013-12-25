#include "DatabaseWidgetTileSet.h"

DatabaseWidgetTileSet::DatabaseWidgetTileSet(const DATABASE::TileSetDatabase* pDatabase, QWidget* pParent) : DatabaseWidgetBase(pParent),
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

void DatabaseWidgetTileSet::setTileDatabaseModel(DatabaseModel<DATABASE::TileDatabase>* pDBModel)
{
    m_pModuleTileList->setModel(pDBModel);
    m_pModuleTileSet->setTileDatabase(dynamic_cast<DATABASE::TileDatabase*>(pDBModel->getDatabase()));
}

void DatabaseWidgetTileSet::setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<const DATABASE::TILE_SET::TileSetPrototype*>(pPrototype))
    {
        auto size = pProto->getTileCount();
        m_pModuleTileSet->resizeTileSetTableColumns(size.x);
        m_pModuleTileSet->resizeTileSetTableRows(size.y);
        for (uint32 row = 0; row < size.y; ++row)
        {
            for (uint32 column = 0; column < size.x; ++column)
                m_pModuleTileSet->setTileID(row, column, pProto->getTileID(UInt32Point(column, row)));
        }
    }
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetTileSet::setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<DATABASE::TILE_SET::TileSetPrototype*>(pPrototype))
    {
        UInt32Point size(m_pModuleTileSet->getTileSetTableColumnCount(), m_pModuleTileSet->getTileSetTableColumnCount());
        pProto->resizeTiles(size);
        for (uint32 row = 0; row < size.y; ++row)
        {
            for (uint32 column = 0; column < size.x; ++column)
                pProto->setTileID(UInt32Point(column, row), m_pModuleTileSet->getTileID(row, column));
        }
    }
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetTileSet::clear()
{
    m_pModuleTileSet->clear();
    DatabaseWidgetBase::clear();
}
