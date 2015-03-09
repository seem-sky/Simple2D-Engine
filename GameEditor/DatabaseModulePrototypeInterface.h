#pragma once

#include <QtWidgets/QWidget>

namespace database
{
    namespace ui
    {
        namespace module
        {
            template <class PROTOTYPE>
            class PrototypeInterface : public QWidget
            {
            public:
                PrototypeInterface(QWidget* pParent = nullptr)
                    : QWidget(pParent)
                {}

                virtual void setup(const PROTOTYPE* pPrototype) = 0;
                virtual void setupFrom(PROTOTYPE* pPrototype) const = 0;

                virtual void clear() = 0;
            };
        }
    }
}
