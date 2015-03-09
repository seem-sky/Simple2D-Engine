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
            class Animation : public Base <AnimationDatabase, prototype::Animation>
            {
            public:
                Animation(const helper::CreatorInterface& creator, database::Model* pAdditionTypeModel, AnimationDatabase& DB, QWidget* pParent);
            };
        }
    }
}
