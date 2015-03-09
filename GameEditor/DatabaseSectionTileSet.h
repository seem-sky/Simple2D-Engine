#pragma once

#include "DatabaseSectionBase.h"
#include <Database/Derived.h>
#include <Database/Prototype/Derived.h>
#include "HelperTileCreator.h"
#include "PrototypeTooltipList.h"

namespace database
{
    namespace ui
    {
        namespace section
        {
            class TileSet : public Base < TileSetDatabase, prototype::TileSet >
            {
                Q_OBJECT
            public:
                TileSet(const helper::CreatorInterface& creator, TileSetDatabase& DB, QWidget* pParent);

            private:
                PrototypeTooltipList* m_pTileList;
            };
        }
    }
}