#pragma once

#include "DatabaseModulePrototypeInterface.h"

namespace database
{
    namespace prototype
    {
        class Sprite;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeTexture;
            class PrototypeSprite : public PrototypeInterface < prototype::Sprite >
            {

            public:
                PrototypeSprite(const QString& directory, const QString& extension, const QString& cutPath, QWidget* pParent = nullptr);

                void setup(const prototype::Sprite* pPrototype) override;
                void setupFrom(prototype::Sprite* pPrototype) const override;

                void clear();

            private:
                PrototypeTexture* m_pModuleTexture = nullptr;
            };
        }
    }
}