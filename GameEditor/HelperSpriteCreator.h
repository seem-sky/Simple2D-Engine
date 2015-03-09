#pragma once

#include <Database/Merger.h>
#include <database/Derived.h>
#include <Database/Prototype/Compare.h>
#include "HelperCreatorInterface.h"

namespace helper
{
    class SpriteCreator : public CreatorInterface
    {
    public:
        SpriteCreator(const database::Merger<database::SpriteDatabase, database::prototype::Sprite, &database::prototype::compare>& spriteDB);

        QPixmap createPixmap(uint32 ID) const override;

        const database::Interface& getDatabase() const override;

    private:
        const database::Merger<database::SpriteDatabase, database::prototype::Sprite, &database::prototype::compare>& m_SpriteDB;
    };
}
