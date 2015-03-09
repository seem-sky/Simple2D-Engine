#pragma once

#include <QtGui/QPixmap>
#include <Global.h>
#include <Database/Interface.h>

namespace helper
{
    class CreatorInterface
    {
    public:
        virtual QPixmap createPixmap(uint32 ID) const = 0;

        virtual const database::Interface& getDatabase() const = 0;
    };
}
