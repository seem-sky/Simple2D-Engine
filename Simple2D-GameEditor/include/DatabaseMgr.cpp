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
    setLocalisationDatabase(new LocalisationDatabase());
    setMapDatabase(new MAP_STRUCTURE::MapDatabase());
}

// overwrite copy constructor
DatabaseMgr::DatabaseMgr(const DatabaseMgr& other)
{
    copyFrom(other);
}

void DatabaseMgr::takeFrom(DatabaseMgr& other, uint32 databases /* = ALL_DATABASES */)
{
    uint32 curDB = 0x1;
    // ignore map database
    databases ^= MAP_DATABASE;
    for (uint32 i = 0; i < m_Databases.size() && i < other.m_Databases.size(); ++i)
    {
        if ((databases & curDB) != 0)
            m_Databases.at(i) = std::unique_ptr<IDatabase>(other.m_Databases.at(i).release());
        curDB <<= 1;
    }
}

void DatabaseMgr::copyFrom(const DatabaseMgr& other, uint32 databases /* = ALL_DATABASES */)
{
    if (databases & TILE_DATABASE)
        setTileDatabase(new TileDatabase(*other.getTileDatabase()));
    if (databases & TILE_SET_DATABASE)
        setTileSetDatabase(new TileSetDatabase(*other.getTileSetDatabase()));
    if (databases & AUTO_TILE_DATABASE)
        setAutoTileDatabase(new AutoTileDatabase(*other.getAutoTileDatabase()));
    if (databases & SPRITE_DATABASE)
        setSpriteDatabase(new SpriteDatabase(*other.getSpriteDatabase()));
    if (databases & ANIMATION_DATABASE)
        setAnimationDatabase(new AnimationDatabase(*other.getAnimationDatabase()));
    if (databases & ANIMATION_TYPE_DATABASE)
        setAnimationTypeDatabase(new AnimationTypeDatabase(*other.getAnimationTypeDatabase()));
    if (databases & LOCALISATION_DATABASE)
        setLocalisationDatabase(new LocalisationDatabase(*other.getLocalisationDatabase()));
    if (databases&  WORLD_OBJECT_DATABASE)
        setWorldObjectDatabase(new WorldObjectDatabase(*other.getWorldObjectDatabase()));

    // ignore MapDatabase
    //if (databases & MAP_DATABASE)
    //    IO::DatabaseReader<MAP_STRUCTURE::MapDatabase>(getMapDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::MAP_DATABASE)));
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

    if (databases & TILE_DATABASE)
        IO::DatabaseReader<TileDatabase>(getTileDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::TILE_DATABASE)));
    if (databases & TILE_SET_DATABASE)
        IO::DatabaseReader<TileSetDatabase>(getTileSetDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::TILE_SET_DATABASE)));
    if (databases & AUTO_TILE_DATABASE)
        IO::DatabaseReader<AutoTileDatabase>(getAutoTileDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::AUTO_TILE_DATABASE)));
    if (databases & SPRITE_DATABASE)
        IO::DatabaseReader<SpriteDatabase>(getSpriteDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::SPRITE_DATABASE)));
    if (databases & ANIMATION_DATABASE)
        IO::DatabaseReader<AnimationDatabase>(getAnimationDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::ANIMATION_DATABASE)));
    if (databases & ANIMATION_TYPE_DATABASE)
        IO::DatabaseReader<AnimationTypeDatabase>(getAnimationTypeDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::ANIMATION_TYPE_DATABASE)));
    if (databases & MAP_DATABASE)
        IO::DatabaseReader<MAP_STRUCTURE::MapDatabase>(getMapDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::MAP_DATABASE)));
    if (databases & LOCALISATION_DATABASE)
        IO::DatabaseReader<LocalisationDatabase>(getLocalisationDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::LOCALISATION_DATABASE)));
    //if (databases&  WORLD_OBJECT_DATABASE)
    //    IO::DatabaseReader<WorldObjectDatabase>(getWorldObjectDatabase()).read(projectPath + DATABASE_FILE.at(DATABASE::WORLD_OBJECT_DATABASE));

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