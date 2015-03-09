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
            class Tile : public Base<TileDatabase, prototype::Tile>
            {
                Q_OBJECT
            public:
                Tile(TileDatabase& DB, QWidget* pParent);

            private slots:
                void _onMassImport(const QStringList&);
            };
        }
    }
}