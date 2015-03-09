#include "HelperTileCreator.h"
#include "Config.h"
#include <QtGlobal.h>

using namespace helper;

TileCreator::TileCreator(const database::Merger<database::TileDatabase, database::prototype::Tile, &database::prototype::compare>& tileDB)
    : CreatorInterface(), m_TileDB(tileDB)
{}

const database::Interface& TileCreator::getDatabase() const
{
    return m_TileDB;
}

QPixmap TileCreator::createPixmap(uint32 ID) const
{
    QPixmap pixmap;
    if (auto pPrototype = m_TileDB.getPrototype(ID))
        ::createPixmap(Config::get()->getProjectDirectory() + pPrototype->getBaseDirectory() + pPrototype->getPathName(), pPrototype->getTransparencyColor(), pixmap);
    return pixmap;
}
