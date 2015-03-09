#include "DatabaseSectionTileSet.h"
#include "moc_DatabaseSectionTileSet.h"
#include "DatabaseModulePrototypeTileSet.h"
#include <QtWidgets/QGridLayout>

using namespace database;
using namespace ui::section;

TileSet::TileSet(const helper::CreatorInterface& creator, TileSetDatabase& DB, QWidget* pParent)
    : Base(DB, new module::PrototypeTileSet(creator), pParent), m_pTileList(new PrototypeTooltipList(creator, pParent))
{
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->addWidget(m_pTileList, 0, 2, -1, 1);
        pLayout->setColumnStretch(1, 1);
        pLayout->setColumnStretch(2, 0);
    }
}
