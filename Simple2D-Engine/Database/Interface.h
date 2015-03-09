#pragma once

#include "Prototype/Base.h"

namespace database
{
    class Interface
    {
    public:
        virtual const prototype::Base* getPrototype(uint32 ID) const = 0;

        virtual uint32 getSize() const = 0;
        virtual void setSize(uint32 size) = 0;
        virtual uint32 getMaximumSize() const = 0;

        virtual bool isEmpty() const = 0;
    };
}
