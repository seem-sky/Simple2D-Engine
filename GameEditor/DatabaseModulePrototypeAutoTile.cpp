#include "DatabaseModulePrototypeAutoTile.h"
#include "DatabaseModulePrototypeBase.h"
#include <Database/Prototype/Derived.h>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>

using namespace database;
using namespace ui::module;

PrototypeAutoTile::PrototypeAutoTile(const helper::CreatorInterface& creator, QWidget* pParent)
    : PrototypeInterface(pParent), Ui_DatabaseModulePrototypeAutoTile()
{
    setupUi(this);

    auto pLayout = new QGridLayout(m_pBasicSet);
    m_pBasicSet->setLayout(pLayout);
    uint32 counter = 0;
    for (auto& ptr : m_TileDropLabels)
    {
        ptr = new TileDropWidget(creator, this);
        ptr->setLineWidth(2);
        ptr->setFrameShape(QFrame::Box);
        ptr->setFrameShadow(QFrame::Raised);
        ptr->setMinimumSize(MAP::TILE_SIZE + ptr->frameWidth() * 2, MAP::TILE_SIZE + ptr->frameWidth() * 2);
        ptr->setMaximumSize(MAP::TILE_SIZE + ptr->frameWidth() * 2, MAP::TILE_SIZE + ptr->frameWidth() * 2);
        ptr->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        if (counter < DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_SET_COUNT - 1)
            pLayout->addWidget(ptr, counter / 3, counter % 3);
        else
        {
            auto pFormLayout = new QFormLayout(m_pInnerCenter);
            m_pInnerCenter->setLayout(pFormLayout);
            pFormLayout->addRow(new QLabel("inner center:", this), ptr);
        }
        ++counter;
    }
}

void PrototypeAutoTile::setup(const prototype::AutoTile* pPrototype)
{
    m_pModuleBase->setup(pPrototype);

    for (uint32 i = 0; i < DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_SET_COUNT; ++i)
        m_TileDropLabels.at(i)->setTileID(pPrototype->getTileID(static_cast<database::prototype::AutoTile::Index>(i)));
}

void PrototypeAutoTile::setupFrom(prototype::AutoTile* pPrototype) const
{
    m_pModuleBase->setupFrom(pPrototype);

    for (uint32 i = 0; i < DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_SET_COUNT; ++i)
        pPrototype->setTileID(static_cast<database::prototype::AutoTile::Index>(i), m_TileDropLabels.at(i)->getTileID());
}

void PrototypeAutoTile::clear()
{
    m_pModuleBase->clear();
    
    for (auto ptr : m_TileDropLabels)
        ptr->setTileID(0);
}
