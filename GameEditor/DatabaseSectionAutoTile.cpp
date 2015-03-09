#include "DatabaseSectionAutoTile.h"
#include "DatabaseModulePrototypeAutoTile.h"

using namespace database;
using namespace ui::section;

AutoTile::AutoTile(const helper::CreatorInterface& creator, AutoTileDatabase& DB, QWidget* pParent)
    : Base(DB, new ui::module::PrototypeAutoTile(creator), pParent), m_pTileList(new PrototypeTooltipList(creator, this))
{
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->addWidget(m_pTileList, 0, 2, -1, 1);
        pLayout->setColumnStretch(2, 0);
        pLayout->setColumnStretch(3, 1);
    }
}
