#include "DatabaseWidgetAutoTile.h"

using namespace DATABASE::PROTOTYPE;

DatabaseWidgetAutoTile::DatabaseWidgetAutoTile(QWidget* pParent) : DatabaseWidgetBase(pParent), m_pModuleAutoTile(new DatabaseModuleAutoTile(this)),
    m_pModuleTileList(new DatabaseModuleTextureDragList(this))
{
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->addWidget(m_pModuleAutoTile, 0, 1);

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
        pLayout->addLayout(pBoxLayout, 0, 2, -1, 1);

        pLayout->setColumnStretch(1, 0);
        pLayout->setColumnStretch(2, 0);
        pLayout->setColumnStretch(3, 1);
    }
}

void DatabaseWidgetAutoTile::setupWidgetsFromPrototype(const Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<const AUTO_TILE::AutoTilePrototype*>(pPrototype))
        m_pModuleAutoTile->setTiles(pProto->getTiles());
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetAutoTile::setupPrototypeFromWidgets(Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<AUTO_TILE::AutoTilePrototype*>(pPrototype))
        pProto->setTiles(m_pModuleAutoTile->getTiles());
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetAutoTile::clear()
{
    m_pModuleAutoTile->clear();
    DatabaseWidgetBase::clear();
}

void DatabaseWidgetAutoTile::setDatabaseMgr(DATABASE::DatabaseMgr& DBMgr)
{
    m_pModuleList->setDatabaseModel(new DATABASE::DatabaseModel(DBMgr, DATABASE::DatabaseType::AUTO_TILE_DATABASE));
    m_pModuleAutoTile->setTileDatabase(DBMgr.getTileDatabase());
}

void DatabaseWidgetAutoTile::setTileDatabaseModel(DATABASE::ConstDatabaseModel* pModel)
{
    m_pModuleTileList->setModel(pModel);
}
