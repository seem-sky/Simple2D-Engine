#include "DatabaseWidgetAutoTile.h"

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

void DatabaseWidgetAutoTile::setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<const DATABASE::AUTO_TILE::AutoTilePrototype*>(pPrototype))
        m_pModuleAutoTile->setTiles(pProto->getTiles());
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetAutoTile::setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<DATABASE::AUTO_TILE::AutoTilePrototype*>(pPrototype))
        pProto->setTiles(m_pModuleAutoTile->getTiles());
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetAutoTile::clear()
{
    m_pModuleAutoTile->clear();
    DatabaseWidgetBase::clear();
}

void DatabaseWidgetAutoTile::setTileDatabaseModel(TileDatabaseModel* pTileDBModel)
{
    m_pModuleAutoTile->setTileDatabase(dynamic_cast<DATABASE::TileDatabase*>(pTileDBModel->getDatabase()));
    m_pModuleTileList->setModel(pTileDBModel);
}
