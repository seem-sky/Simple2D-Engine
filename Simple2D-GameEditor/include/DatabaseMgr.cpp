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
    m_pTileDatabase = TileDatabasePtr(new TileDatabase());
    m_pTileSetDatabase = TileSetDatabasePtr(new TileSetDatabase());
    m_pAutoTileDatabase = AutoTileDatabasePtr(new AutoTileDatabase());
    m_pSpriteDatabase = SpriteDatabasePtr(new SpriteDatabase());
    m_pAnimationDatabase = AnimationDatabasePtr(new AnimationDatabase());
    m_pWorldObjectDatabase = WorldObjectDatabasePtr(new WorldObjectDatabase());
    m_pDynamicObjectDatabase = DynamicObjectDatabasePtr(new DynamicObjectDatabase());
    m_pObjectAnimationTypeDatabase = ObjectAnimationTypeDatabasePtr(new ObjectAnimationTypeDatabase());
    m_pMapDatabase = MapDatabasePtr(new MAP_STRUCTURE::MapDatabase());
    m_pLocalsDatabase = LocalsDatabasePtr(new LocalsDatabase());
}

void DatabaseMgr::clear()
{
    if (m_pTileDatabase) m_pTileDatabase->clear();
    if (m_pTileSetDatabase) m_pTileSetDatabase->clear();
    if (m_pAutoTileDatabase) m_pAutoTileDatabase->clear();
    if (m_pSpriteDatabase) m_pSpriteDatabase->clear();
    if (m_pAnimationDatabase) m_pAnimationDatabase->clear();
    if (m_pWorldObjectDatabase) m_pWorldObjectDatabase->clear();
    if (m_pDynamicObjectDatabase) m_pDynamicObjectDatabase->clear();
    if (m_pObjectAnimationTypeDatabase) m_pObjectAnimationTypeDatabase->clear();
    if (m_pMapDatabase) m_pMapDatabase->clear();
    if (m_pLocalsDatabase) m_pLocalsDatabase->clear();
}

bool DatabaseMgr::loadDatabase(const QString &projectPath, uint32 databases)
{
    clear();
    QDir projectDir(projectPath);
    if (!projectDir.exists())
        return false;

    if (databases & TILE_DATABASE)
        IO::DatabaseReader<TilePrototype>(getTileDatabase()).read(projectPath + TILE_DATABASE_PATH);
    if (databases & TILE_SET_DATABASE)
        IO::DatabaseReader<TILE_SET::TileSetPrototype>(getTileSetDatabase()).read(projectPath + TILE_SET_DATABASE_PATH);
    if (databases & AUTO_TILE_DATABASE)
        IO::DatabaseReader<AUTO_TILE::AutoTilePrototype>(getAutoTileDatabase()).read(projectPath + AUTO_TILE_DATABASE_PATH);
    if (databases & SPRITE_DATABASE)
        IO::DatabaseReader<SpritePrototype>(getSpriteDatabase()).read(projectPath + SPRITE_DATABASE_PATH);
    if (databases & ANIMATION_DATABASE)
        IO::DatabaseReader<AnimationPrototype>(getAnimationDatabase()).read(projectPath + ANIMATION_DATABASE_PATH);
    if (databases & OBJECT_ANIMATION_TYPE_DATABASE)
        IO::DatabaseReader<ObjectAnimationTypePrototype>(getObjectAnimationTypeDatabase()).read(projectPath + OBJECT_ANIMATION_TYPE_DATABASE_PATH);
    if (databases & MAP_DATABASE)
        IO::DatabaseReader<MAP_STRUCTURE::MapPrototype>(getMapDatabase()).read(projectPath + MAP_DATABASE_PATH);
    if (databases & LOCALS_DATABASE)
        IO::DatabaseReader<LocalisationPrototype>(getLocalsDatabase()).read(projectPath + LOCALS_DATABASE_PATH);
    if (databases & WORLD_OBJECT_DATABASE)
        IO::DatabaseReader<MAP_OBJECT::WorldObjectPrototype>(getWorldObjectDatabase()).read(projectPath + WORLD_OBJECT_DATABASE_PATH);
    return true;
}

bool DatabaseMgr::saveDatabase(const QString &projectPath, uint32 databases)
{
    QDir projectDir(projectPath);
    if (!projectDir.exists())
        return false;

    if (databases & TILE_DATABASE)
        IO::DatabaseWriter<TilePrototype>(getTileDatabase()).write(projectPath + TILE_DATABASE_PATH);
    if (databases & TILE_SET_DATABASE)
        IO::DatabaseWriter<TILE_SET::TileSetPrototype>(getTileSetDatabase()).write(projectPath + TILE_SET_DATABASE_PATH);
    if (databases & AUTO_TILE_DATABASE)
        IO::DatabaseWriter<AUTO_TILE::AutoTilePrototype>(getAutoTileDatabase()).write(projectPath + AUTO_TILE_DATABASE_PATH);
    if (databases & SPRITE_DATABASE)
        IO::DatabaseWriter<SpritePrototype>(getSpriteDatabase()).write(projectPath + SPRITE_DATABASE_PATH);
    if (databases & ANIMATION_DATABASE)
        IO::DatabaseWriter<AnimationPrototype>(getAnimationDatabase()).write(projectPath + ANIMATION_DATABASE_PATH);
    if (databases & OBJECT_ANIMATION_TYPE_DATABASE)
        IO::DatabaseWriter<ObjectAnimationTypePrototype>(getObjectAnimationTypeDatabase()).write(projectPath + OBJECT_ANIMATION_TYPE_DATABASE_PATH);
    if (databases & MAP_DATABASE)
        IO::DatabaseWriter<MAP_STRUCTURE::MapPrototype>(getMapDatabase()).write(projectPath + MAP_DATABASE_PATH);
    if (databases & LOCALS_DATABASE)
        IO::DatabaseWriter<LocalisationPrototype>(getLocalsDatabase()).write(projectPath + LOCALS_DATABASE_PATH);
    if (databases & WORLD_OBJECT_DATABASE)
        IO::DatabaseWriter<MAP_OBJECT::WorldObjectPrototype>(getWorldObjectDatabase()).write(projectPath + WORLD_OBJECT_DATABASE_PATH);
    return true;
}

QPixmap TILE_SET::createTileSetPixmap(const QString &path, ConstTileSetPrototypePtr proto, ConstTileDatabasePtr tileDB)
{
    if (!proto || !tileDB)
        return std::move(QPixmap());

    QPixmap pixmap(proto->getTileCount().x*TILE_SIZE, proto->getTileCount().y*TILE_SIZE);
    pixmap.fill();
    QPainter painter(&pixmap);
    for (uint32 x = 0; x < proto->getTileCount().x; ++x)
    {
        for (uint32 y = 0; y < proto->getTileCount().y; ++y)
        {
            ConstTilePrototypePtr tileProto;
            QPixmap tempPixmap;
            if (tileDB->getItem(proto->getTileID(UInt32Point(x,y)), tileProto) && createPixmapFromTexturePrototype(path, tileProto, tempPixmap))
                painter.drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, tempPixmap);
        }
    }
    return pixmap;
}