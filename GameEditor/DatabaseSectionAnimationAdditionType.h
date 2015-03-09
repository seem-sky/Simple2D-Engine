#pragma once

#include "DatabaseSectionBase.h"
#include <Database/Derived.h>
#include <Database/Prototype/Derived.h>

namespace database
{
    namespace ui
    {
        namespace section
        {
            class AnimationAdditionType : public Base < AnimationAdditionTypeDatabase, prototype::AnimationAdditionType >
            {
            public:
                AnimationAdditionType(AnimationAdditionTypeDatabase& DB, QWidget* pParent);
            };
        }
    }
}
