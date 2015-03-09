#pragma once

#include "DatabaseModulePrototypeInterface.h"

namespace database
{
    namespace prototype
    {
        class AnimationAdditionType;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeBase;
            class PrototypeAnimationAdditionType : public PrototypeInterface < prototype::AnimationAdditionType >
            {

            public:
                PrototypeAnimationAdditionType(QWidget* pParent = nullptr);

                void setup(const prototype::AnimationAdditionType* pPrototype) override;
                void setupFrom(prototype::AnimationAdditionType* pPrototype) const override;

                void clear();

            private:
                PrototypeBase* m_pModuleBase = nullptr;
            };
        }
    }
}