#pragma once

#include <Database/Merger.h>
#include <database/Derived.h>
#include <Database/Prototype/Compare.h>
#include "HelperCreatorInterface.h"

namespace helper
{
    class TileCreator : public CreatorInterface
    {
    public:
        TileCreator(const database::Merger<database::TileDatabase, database::prototype::Tile, &database::prototype::compare>& tileDB);

        QPixmap createPixmap(uint32 ID) const override;

        const database::Interface& getDatabase() const override;

    private:
        const database::Merger<database::TileDatabase, database::prototype::Tile, &database::prototype::compare>& m_TileDB;
    };
}
