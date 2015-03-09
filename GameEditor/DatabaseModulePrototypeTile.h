#pragma once

#include "DatabaseModulePrototypeInterface.h"
#include <QtWidgets/QPushButton>
#include <array>

namespace database
{
    namespace prototype
    {
        class Tile;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeTexture;
            class PrototypeTile : public PrototypeInterface<prototype::Tile>
            {
                Q_OBJECT
            public:
                PrototypeTile(const QString& directory, const QString& extension, const QString& cutPath, QWidget* pParent = nullptr);

                void setup(const prototype::Tile* pPrototype) override;
                void setupFrom(prototype::Tile* pPrototype) const override;

                void clear();

            private slots:
                void _onPassabilityButtonClicked();

            private:
                PrototypeTexture* m_pModuleTexture = nullptr;
                std::array<QPushButton*, 4> m_Buttons;
            };
        }
    }
}