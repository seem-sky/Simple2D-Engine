#pragma once

#include "DatabaseModulePrototypeAnimationAddition.h"

namespace database
{
    namespace prototype
    {
        class AnimationAdditionPoint;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeAnimationAdditionPoint : public PrototypeInterface<prototype::AnimationAdditionPoint>
            {

            public:
                PrototypeAnimationAdditionPoint(const helper::CreatorInterface& creator, database::Model* pTypeModel, QWidget* pParent = nullptr);

                void setup(const prototype::AnimationAdditionPoint* pPrototype);
                void setupFrom(prototype::AnimationAdditionPoint* pPrototype) const;

                void clear();

            private:
                PrototypeAnimationAddition* m_pModuleAddition;
            };
        }
    }
}
