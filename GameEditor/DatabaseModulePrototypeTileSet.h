#pragma once

#include "DatabaseModulePrototypeInterface.h"
#include "HelperTileCreator.h"
#include "UI/UI_DatabaseModulePrototypeTileSet.h"

namespace database
{
    namespace prototype
    {
        class TileSet;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeBase;
            class PrototypeTileSet : public PrototypeInterface < prototype::TileSet >, Ui_DatabaseModulePrototypeTileSet
            {
                Q_OBJECT
            public:
                PrototypeTileSet(const helper::CreatorInterface& creator, QWidget* pParent = nullptr);

                void setup(const prototype::TileSet* pPrototype) override;
                void setupFrom(prototype::TileSet* pPrototype) const override;

                void clear();

                void setSize(const GEOMETRY::Size<uint32>& size);
                GEOMETRY::Size<uint32> getSize() const;
                void setWidth(uint32 width);
                uint32 getWidth() const;
                void setHeight(uint32 height);
                uint32 getHeight() const;

                uint32 getTileID(uint32 row, uint32 column) const;
                void setTileID(uint32 row, uint32 column, uint32 ID);

            private slots:
                void _onHorizontalAddClicked();
                void _onVerticalAddClicked();
                void _onHorizontalRemoveClicked();
                void _onVerticalRemoveClicked();

            private:
                const helper::CreatorInterface& m_Creator;
            };
        }
    }
}
