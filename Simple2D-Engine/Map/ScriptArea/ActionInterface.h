#ifndef SCRIPT_AREA_ACTION_INTERFACE_H
#define SCRIPT_AREA_ACTION_INTERFACE_H

#include "ActionData.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        namespace ACTION
        {
            class Interface
            {
            public:
                Interface() = default;

                virtual Type getType() const = 0;
                virtual Data getData() const = 0;
            };
        }
    }
}
#endif
