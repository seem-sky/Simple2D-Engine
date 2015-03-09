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
            class Sprite : public Base < SpriteDatabase, prototype::Sprite >
            {
                Q_OBJECT
            public:
                Sprite(SpriteDatabase& DB, QWidget* pParent);

            private slots:
                void _onMassImport(const QStringList&);
            };
        }
    }
}
