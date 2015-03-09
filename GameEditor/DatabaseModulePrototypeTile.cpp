#include "DatabaseModulePrototypeTile.h"
#include "moc_DatabaseModulePrototypeTile.h"
#include "DatabaseModulePrototypeTexture.h"
#include <Database/Prototype/Derived.h>

using namespace database;
using namespace ui::module;

const char* BUTTON_ICONS[]
{
    "icons/arrow-up.png",
    "icons/arrow-left.png",
    "icons/arrow-right.png",
    "icons/arrow-down.png"
};
const char* PASSABLE_NONE_ICON  = "icons/cross.png";

enum PassabilityButton
{
    PASSABILITY_UP,
    PASSABILITY_LEFT,
    PASSABILITY_RIGHT,
    PASSABILITY_DOWN
};

void updateButtons(std::array<QPushButton*, 4>& buttons)
{
    for (uint32 i = 0; i < 4; ++i)
        buttons.at(i)->setIcon(QIcon(buttons.at(i)->isChecked() ? BUTTON_ICONS[i] : PASSABLE_NONE_ICON));
}

PrototypeTile::PrototypeTile(const QString& directory, const QString& extension, const QString& cutPath, QWidget* pParent)
    : PrototypeInterface(pParent), m_pModuleTexture(new PrototypeTexture(this))
{
    auto pLayout = new QGridLayout();
    pLayout->setContentsMargins(0, 0, 0, 0);
    m_pModuleTexture->setFileDialogInfos(directory, extension, cutPath);
    pLayout->addWidget(m_pModuleTexture);
    setLayout(pLayout);

    auto pViewLayout = new QGridLayout();
    pViewLayout->setContentsMargins(0, 0, 0, 0);
    m_pModuleTexture->getTextureView()->setLayout(pViewLayout);
    
    //setup buttons
    for (auto& pButton : m_Buttons)
    {
        pButton = new QPushButton(this);
        pButton->setMaximumSize(32, 32);
        pButton->setMinimumSize(32, 32);
        pButton->setCheckable(true);
        connect(pButton, SIGNAL(clicked()), this, SLOT(_onPassabilityButtonClicked()));
    }

    pViewLayout->addWidget(m_Buttons.at(PASSABILITY_UP), 0, 1);
    pViewLayout->addWidget(m_Buttons.at(PASSABILITY_LEFT), 1, 0);
    pViewLayout->addWidget(m_Buttons.at(PASSABILITY_RIGHT), 1, 2);
    pViewLayout->addWidget(m_Buttons.at(PASSABILITY_DOWN), 2, 1);
    
    updateButtons(m_Buttons);
}

void PrototypeTile::setup(const prototype::Tile* pPrototype)
{
    m_pModuleTexture->setup(pPrototype);

    auto flagMgr = pPrototype->getPassability();
    m_Buttons.at(PASSABILITY_UP)->setChecked(flagMgr.hasFlag(prototype::Tile::PassabilityFlag::PASSABLE_UP));
    m_Buttons.at(PASSABILITY_LEFT)->setChecked(flagMgr.hasFlag(prototype::Tile::PassabilityFlag::PASSABLE_LEFT));
    m_Buttons.at(PASSABILITY_DOWN)->setChecked(flagMgr.hasFlag(prototype::Tile::PassabilityFlag::PASSABLE_DOWN));
    m_Buttons.at(PASSABILITY_RIGHT)->setChecked(flagMgr.hasFlag(prototype::Tile::PassabilityFlag::PASSABLE_RIGHT));
    updateButtons(m_Buttons);
}

void PrototypeTile::setupFrom(prototype::Tile* pPrototype) const
{
    m_pModuleTexture->setupFrom(pPrototype);

    auto& flagMgr = pPrototype->getPassability();
    flagMgr.setFlag(prototype::Tile::PassabilityFlag::PASSABLE_LEFT, m_Buttons.at(PASSABILITY_LEFT)->isChecked());
    flagMgr.setFlag(prototype::Tile::PassabilityFlag::PASSABLE_UP, m_Buttons.at(PASSABILITY_UP)->isChecked());
    flagMgr.setFlag(prototype::Tile::PassabilityFlag::PASSABLE_RIGHT, m_Buttons.at(PASSABILITY_RIGHT)->isChecked());
    flagMgr.setFlag(prototype::Tile::PassabilityFlag::PASSABLE_DOWN, m_Buttons.at(PASSABILITY_DOWN)->isChecked());
    pPrototype->setPassability(flagMgr);
}

void PrototypeTile::clear()
{
    m_pModuleTexture->clear();
    for (auto pButton : m_Buttons)
        pButton->setChecked(false);
}

void PrototypeTile::_onPassabilityButtonClicked()
{
    updateButtons(m_Buttons);
}
