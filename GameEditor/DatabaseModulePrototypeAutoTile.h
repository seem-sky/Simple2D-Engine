#pragma once

#include "DatabaseModulePrototypeInterface.h"
#include "UI/UI_DatabaseModulePrototypeAutoTile.h"
#include <array>
#include "TileDropWidget.h"
#include "Simple2D-Global.h"

namespace database
{
    namespace prototype
    {
        class AutoTile;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeAutoTile : public PrototypeInterface<prototype::AutoTile>, Ui_DatabaseModulePrototypeAutoTile
            {
            public:
                PrototypeAutoTile(const helper::CreatorInterface& creator, QWidget* pParent = nullptr);

                void setup(const prototype::AutoTile* pPrototype) override;
                void setupFrom(prototype::AutoTile* pPrototype) const override;

                void clear();

            private:
                std::array<TileDropWidget*, DATABASE::PROTOTYPE::AUTO_TILE::AUTO_TILE_SET_COUNT> m_TileDropLabels;
            };
        }
    }
}