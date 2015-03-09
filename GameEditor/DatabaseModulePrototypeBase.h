#pragma once

#include "DatabaseModulePrototypeInterface.h"
#include "UI/UI_DatabaseModulePrototypeBase.h"
#include <Global.h>

namespace database
{
    namespace prototype
    {
        class Base;
    }

    namespace ui
    {
        namespace module
        {
            class PrototypeBase : public PrototypeInterface<prototype::Base>, Ui_DatabaseModulePrototypeBase
            {
            public:
                PrototypeBase(QWidget* pParent = nullptr);

                void setup(const prototype::Base* pPrototype) override;
                void setupFrom(prototype::Base* pPrototype) const override;

                void clear();
            };
        }
    }
}