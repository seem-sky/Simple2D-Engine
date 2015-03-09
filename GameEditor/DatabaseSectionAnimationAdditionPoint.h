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
            class AnimationAdditionPoint : public Base <AnimationAdditionPointDatabase, prototype::AnimationAdditionPoint>
            {
            public:
                AnimationAdditionPoint(const helper::CreatorInterface& creator, database::Model* pAnimationAdditionTypeModel, AnimationAdditionPointDatabase& DB,
                    QWidget* pParent);
            };
        }
    }
}
