#pragma once

#include "DatabaseSectionBase.h"
#include <Database/Derived.h>
#include <Database/Prototype/Derived.h>
#include "PrototypeTooltipList.h"

namespace database
{
    namespace ui
    {
        namespace section
        {
            class AutoTile : public Base<AutoTileDatabase, prototype::AutoTile>
            {
            public:
                AutoTile(const helper::CreatorInterface& creator, AutoTileDatabase& DB, QWidget* pParent);

            private:
                PrototypeTooltipList* m_pTileList;
            };
        }
    }
}