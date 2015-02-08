#include "DatabaseMgr.h"
#include <QtCore/QDir>
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
    setMapDatabase(new MapDatabase());
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
        IO::DatabaseReader<MapDatabase>(getMapDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::MAP_DATABASE)));
    if (databases & LOCALISATION_DATABASE)
        IO::DatabaseReader<LocalisationDatabase>(getLocalisationDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::LOCALISATION_DATABASE)));
    if (databases&  WORLD_OBJECT_DATABASE)
        IO::DatabaseReader<WorldObjectDatabase>(getWorldObjectDatabase()).read(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::WORLD_OBJECT_DATABASE)));

    return true;
}

bool DatabaseMgr::saveDatabase(const QString& projectPath, uint32 databases)
{
    QDir projectDir(projectPath);
    if (!projectDir.exists())
        return false;

    if (databases&  TILE_DATABASE)
        IO::DatabaseWriter<TileDatabase>(getTileDatabase()).write(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::TILE_DATABASE)));
    if (databases&  TILE_SET_DATABASE)
        IO::DatabaseWriter<TileSetDatabase>(getTileSetDatabase()).write(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::TILE_SET_DATABASE)));
    if (databases&  AUTO_TILE_DATABASE)
        IO::DatabaseWriter<AutoTileDatabase>(getAutoTileDatabase()).write(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::AUTO_TILE_DATABASE)));
    if (databases&  SPRITE_DATABASE)
        IO::DatabaseWriter<SpriteDatabase>(getSpriteDatabase()).write(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::SPRITE_DATABASE)));
    if (databases&  ANIMATION_DATABASE)
        IO::DatabaseWriter<AnimationDatabase>(getAnimationDatabase()).write(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::ANIMATION_DATABASE)));
    if (databases&  ANIMATION_TYPE_DATABASE)
        IO::DatabaseWriter<AnimationTypeDatabase>(getAnimationTypeDatabase()).write(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::ANIMATION_TYPE_DATABASE)));
    if (databases&  MAP_DATABASE)
        IO::DatabaseWriter<MapDatabase>(getMapDatabase()).write(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::MAP_DATABASE)));
    if (databases&  LOCALISATION_DATABASE)
        IO::DatabaseWriter<LocalisationDatabase>(getLocalisationDatabase()).write(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::LOCALISATION_DATABASE)));
    if (databases&  WORLD_OBJECT_DATABASE)
        IO::DatabaseWriter<WorldObjectDatabase>(getWorldObjectDatabase()).write(projectPath + DATABASE_FILE.at(static_cast<uint32>(DatabaseType::WORLD_OBJECT_DATABASE)));
    return true;
}

QPixmap PROTOTYPE::TILE_SET::createTileSetPixmap(const QString& path, const TileSetPrototype* pSet, const TileDatabase* pTileDB)
{
    if (!pSet || !pTileDB)
        return std::move(QPixmap());

    QPixmap pixmap(pSet->getTileSetSize().getX()*MAP::TILE_SIZE, pSet->getTileSetSize().getY()*MAP::TILE_SIZE);
    pixmap.fill();
    QPainter painter(&pixmap);
    GEOMETRY::Point<uint32> pos;
    for (pos.getX() = 0; pos.getX() < pSet->getTileSetSize().getX(); ++pos.getX())
    {
        for (pos.getY() = 0; pos.getY() < pSet->getTileSetSize().getY(); ++pos.getY())
        {
            auto pTileProto = pTileDB->getOriginalPrototype(pSet->getTileID(pos));
            QPixmap tempPixmap;
            if (pTileProto && createPixmapFromTexturePrototype(path, pTileProto, tempPixmap))
                painter.drawTiledPixmap(pos.getX()*MAP::TILE_SIZE, pos.getY()*MAP::TILE_SIZE, MAP::TILE_SIZE, MAP::TILE_SIZE, tempPixmap);
        }
    }
    return pixmap;
}