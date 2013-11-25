#include "DatabaseWidgetTile.h"
#include "moc_DatabaseWidgetTile.h"

const char PASSABLE_RIGHT_ICON[]    = "icons/arrow-right.png";
const char PASSABLE_LEFT_ICON[]     = "icons/arrow-left.png";
const char PASSABLE_UP_ICON[]       = "icons/arrow-up.png";
const char PASSABLE_DOWN_ICON[]     = "icons/arrow-down.png";
const char PASSABLE_NONE_ICON[]     = "icons/cross.png";

enum PassabilityButton
{
    PASSABILITY_UP,
    PASSABILITY_LEFT,
    PASSABILITY_RIGHT,
    PASSABILITY_DOWN
};

DatabaseWidgetTile::DatabaseWidgetTile(QWidget *pParent) : DatabaseWidgetTexture(pParent)
{
    auto *pNewLayout = new QGridLayout();
    m_pModuleTexture->getTextureView()->setLayout(pNewLayout);
    //setup buttons
    for (auto &pButton : m_Buttons)
    {
        pButton = new QPushButton(this);
        pButton->setMaximumSize(32, 32);
        pButton->setMinimumSize(32, 32);
        pButton->setCheckable(true);
        connect(pButton, SIGNAL(clicked()), this, SLOT(_onPassabilityButtonClicked()));
    }

    pNewLayout->addWidget(m_Buttons.at(PASSABILITY_UP), 0, 1);
    pNewLayout->addWidget(m_Buttons.at(PASSABILITY_LEFT), 1, 0);
    pNewLayout->addWidget(m_Buttons.at(PASSABILITY_RIGHT), 1, 2);
    pNewLayout->addWidget(m_Buttons.at(PASSABILITY_DOWN), 2, 1);
    _updatePassabilityIcons();
}

void DatabaseWidgetTile::setupWidgetsFromPrototype(const DATABASE::Prototype *pPrototype)
{
    if (auto pProto = dynamic_cast<const DATABASE::TilePrototype*>(pPrototype))
        setDataPassability(pProto->getPassability());
    DatabaseWidgetTexture::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetTile::setupPrototypeFromWidgets(DATABASE::Prototype *pPrototype)
{
    if (auto pProto = dynamic_cast<DATABASE::TilePrototype*>(pPrototype))
        pProto->addPassabilityFlag(getDataPassability());
    DatabaseWidgetTexture::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetTile::clear()
{
    setDataPassability(0);
    DatabaseWidgetTexture::clear();
}

void DatabaseWidgetTile::_onPassabilityButtonClicked()
{
    _updatePassabilityIcons();
}

uint8 DatabaseWidgetTile::getDataPassability() const
{
    uint8 result = 0;
    if (m_Buttons.at(PASSABILITY_UP)->isChecked())
        result |= DATABASE::TilePrototype::PASSABLE_UP;
    if (m_Buttons.at(PASSABILITY_RIGHT)->isChecked())
        result |= DATABASE::TilePrototype::PASSABLE_RIGHT;
    if (m_Buttons.at(PASSABILITY_LEFT)->isChecked())
        result |= DATABASE::TilePrototype::PASSABLE_LEFT;
    if (m_Buttons.at(PASSABILITY_DOWN)->isChecked())
        result |= DATABASE::TilePrototype::PASSABLE_DOWN;
    return result;
}

void DatabaseWidgetTile::setDataPassability(uint8 passability)
{
    m_Buttons.at(PASSABILITY_UP)->setChecked(passability & DATABASE::TilePrototype::PASSABLE_UP);
    m_Buttons.at(PASSABILITY_LEFT)->setChecked(passability & DATABASE::TilePrototype::PASSABLE_LEFT);
    m_Buttons.at(PASSABILITY_RIGHT)->setChecked(passability & DATABASE::TilePrototype::PASSABLE_RIGHT);
    m_Buttons.at(PASSABILITY_DOWN)->setChecked(passability & DATABASE::TilePrototype::PASSABLE_DOWN);
    _updatePassabilityIcons();
}

void DatabaseWidgetTile::_updatePassabilityIcons()
{
    if (m_Buttons.at(PASSABILITY_UP)->isChecked())
        m_Buttons.at(PASSABILITY_UP)->setIcon(QIcon(PASSABLE_UP_ICON));
    else
        m_Buttons.at(PASSABILITY_UP)->setIcon(QIcon(PASSABLE_NONE_ICON));

    if (m_Buttons.at(PASSABILITY_RIGHT)->isChecked())
        m_Buttons.at(PASSABILITY_RIGHT)->setIcon(QIcon(PASSABLE_RIGHT_ICON));
    else
        m_Buttons.at(PASSABILITY_RIGHT)->setIcon(QIcon(PASSABLE_NONE_ICON));

    if (m_Buttons.at(PASSABILITY_LEFT)->isChecked())
        m_Buttons.at(PASSABILITY_LEFT)->setIcon(QIcon(PASSABLE_LEFT_ICON));
    else
        m_Buttons.at(PASSABILITY_LEFT)->setIcon(QIcon(PASSABLE_NONE_ICON));

    if (m_Buttons.at(PASSABILITY_DOWN)->isChecked())
        m_Buttons.at(PASSABILITY_DOWN)->setIcon(QIcon(PASSABLE_DOWN_ICON));
    else
        m_Buttons.at(PASSABILITY_DOWN)->setIcon(QIcon(PASSABLE_NONE_ICON));
}