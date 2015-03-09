#include "HelperSpriteCreator.h"
#include "Config.h"
#include <QtGlobal.h>

using namespace helper;

SpriteCreator::SpriteCreator(const database::Merger<database::SpriteDatabase, database::prototype::Sprite, &database::prototype::compare>& spriteDB)
    : CreatorInterface(), m_SpriteDB(spriteDB)
{}

const database::Interface& SpriteCreator::getDatabase() const
{
    return m_SpriteDB;
}

QPixmap SpriteCreator::createPixmap(uint32 ID) const
{
    QPixmap pixmap;
    if (auto pPrototype = m_SpriteDB.getPrototype(ID))
        ::createPixmap(Config::get()->getProjectDirectory() + pPrototype->getBaseDirectory() + pPrototype->getPathName(), pPrototype->getTransparencyColor(), pixmap);
    return pixmap;
}
