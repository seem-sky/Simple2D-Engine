#include "DatabaseMgr.h"
#include <QtCore/QDir>
#include <boost/thread.hpp>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include "QtGlobal.h"
#include "Database_IO.h"

using namespace DATABASE;

DatabaseMgr::DatabaseMgr()
{
    setTileDatabase(new TileDatabase());
    setTileSetDatabase(new TileSetDatabase());
    setAutoTileDatabase(new AutoTileDatabase());
    setSpriteDatabase(new SpriteDatabase());
    setAnimationDatabase(new AnimationDatabase());
    setAnimationTypeDatabase(new AnimationTypeDatabase());
    setWorldObjectDatabase(new WorldObjectDatabase());
    setDynamicObjectDatabase(new DynamicObjectDatabase());
    setLocalisationDatabase(new LocalisationDatabase());
    setMapDatabase(new MAP_STRUCTURE::MapDatabase());
}

void DatabaseMgr::clear()
{
    for (auto& db : m_Databases)
        db->clear();
}

bool DatabaseMgr::loadDatabase(const QString& projectPath, uint32 databases)
{
    clear();
    QDir projectDir(projectPath);
    if (!projectDir.exists())
        return false;

    if (databases&  TILE_DATABASE)
        IO::DatabaseReader<TileDatabase>(getTileDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::TILE_DATABASE));
    if (databases&  TILE_SET_DATABASE)
        IO::DatabaseReader<TileSetDatabase>(getTileSetDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::TILE_SET_DATABASE));
    if (databases&  AUTO_TILE_DATABASE)
        IO::DatabaseReader<AutoTileDatabase>(getAutoTileDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::AUTO_TILE_DATABASE));
    if (databases&  SPRITE_DATABASE)
        IO::DatabaseReader<SpriteDatabase>(getSpriteDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::SPRITE_DATABASE));
    if (databases&  ANIMATION_DATABASE)
        IO::DatabaseReader<AnimationDatabase>(getAnimationDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::ANIMATION_DATABASE));
    if (databases&  ANIMATION_TYPE_DATABASE)
        IO::DatabaseReader<AnimationTypeDatabase>(getAnimationTypeDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::ANIMATION_TYPE_DATABASE));
    if (databases&  MAP_DATABASE)
        IO::DatabaseReader<MAP_STRUCTURE::MapDatabase>(getMapDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::MAP_DATABASE));
    if (databases&  LOCALS_DATABASE)
        IO::DatabaseReader<LocalisationDatabase>(getLocalisationDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::LOCALISATION_DATABASE));
    //if (databases&  WORLD_OBJECT_DATABASE)
    //    IO::DatabaseReader<WorldObjectDatabase>(getWorldObjectDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::WORLD_OBJECT_DATABASE));
    //if (databases&  DYNAMIC_OBJECT_DATABASE)
    //    IO::DatabaseReader<MAP_OBJECT::DynamicObjectPrototype>(getDynamicObjectDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::TILE_DATABASE));
    return true;
}

bool DatabaseMgr::saveDatabase(const QString& projectPath, uint32 databases)
{
    QDir projectDir(projectPath);
    if (!projectDir.exists())
        return false;

    //if (databases&  TILE_DATABASE)
    //    IO::DatabaseWriter<TilePrototype>(getTileDatabase()).write(projectPath + TILE_DATABASE_PATH);
    //if (databases&  TILE_SET_DATABASE)
    //    IO::DatabaseWriter<TILE_SET::TileSetPrototype>(getTileSetDatabase()).write(projectPath + TILE_SET_DATABASE_PATH);
    //if (databases&  AUTO_TILE_DATABASE)
    //    IO::DatabaseWriter<AUTO_TILE::AutoTilePrototype>(getAutoTileDatabase()).write(projectPath + AUTO_TILE_DATABASE_PATH);
    //if (databases&  SPRITE_DATABASE)
    //    IO::DatabaseWriter<SpritePrototype>(getSpriteDatabase()).write(projectPath + SPRITE_DATABASE_PATH);
    //if (databases&  ANIMATION_DATABASE)
    //    IO::DatabaseWriter<AnimationPrototype>(getAnimationDatabase()).write(projectPath + ANIMATION_DATABASE_PATH);
    //if (databases&  OBJECT_ANIMATION_TYPE_DATABASE)
    //    IO::DatabaseWriter<AnimationTypePrototype>(getObjectAnimationTypeDatabase()).write(projectPath + OBJECT_ANIMATION_TYPE_DATABASE_PATH);
    //if (databases&  MAP_DATABASE)
    //    IO::DatabaseWriter<MAP_STRUCTURE::MapPrototype>(getMapDatabase()).write(projectPath + MAP_DATABASE_PATH);
    //if (databases&  LOCALS_DATABASE)
    //    IO::DatabaseWriter<LocalisationPrototype>(getLocalsDatabase()).write(projectPath + LOCALS_DATABASE_PATH);
    //if (databases&  WORLD_OBJECT_DATABASE)
    //    IO::DatabaseWriter<MAP_OBJECT::WorldObjectPrototype>(getWorldObjectDatabase()).write(projectPath + WORLD_OBJECT_DATABASE_PATH);
    //if (databases&  DYNAMIC_OBJECT_DATABASE)
    //    IO::DatabaseWriter<MAP_OBJECT::DynamicObjectPrototype>(getDynamicObjectDatabase()).write(projectPath + DYNAMIC_OBJECT_DATABASE_PATH);
    return true;
}

QPixmap TILE_SET::createTileSetPixmap(const QString& path, const TileSetPrototype* pSet, const TileDatabase* pTileDB)
{
    if (!pSet || !pTileDB)
        return std::move(QPixmap());

    QPixmap pixmap(pSet->getTileCount().x*TILE_SIZE, pSet->getTileCount().y*TILE_SIZE);
    pixmap.fill();
    QPainter painter(&pixmap);
    UInt32Point pos;
    for (pos.x = 0; pos.x < pSet->getTileCount().x; ++pos.x)
    {
        for (pos.y = 0; pos.y < pSet->getTileCount().y; ++pos.y)
        {
            auto pTileProto = pTileDB->getOriginalPrototype(pSet->getTileID(pos));
            QPixmap tempPixmap;
            if (pTileProto && createPixmapFromTexturePrototype(path, pTileProto, tempPixmap))
                painter.drawTiledPixmap(pos.x*TILE_SIZE, pos.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, tempPixmap);
        }
    }
    return pixmap;
}