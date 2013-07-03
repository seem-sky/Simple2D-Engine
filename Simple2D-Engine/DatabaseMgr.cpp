#include "DatabaseMgr.h"
#include <QtCore/QDir>
#include "DatabaseIO.h"
#include <boost/thread.hpp>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include "QtGlobal.h"

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
    m_pMapDatabase = MapDatabasePtr(new MAP::MapDatabase());
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
    // tile database
    TileDatabaseXMLReader tileReader(getTileDatabase());
    if (databases & TILE_DATABASE)
        tileReader.execThreaded(projectPath + TILE_DATABASE_PATH, "TileDatabase");

    // tileset database
    TileSetDatabaseXMLReader tileSetReader(getTileSetDatabase());
    if (databases & TILE_SET_DATABASE)
        tileSetReader.execThreaded(projectPath + TILE_SET_DATABASE_PATH, "TileSetDatabase");

    // autotile database
    AutoTileDatabaseXMLReader autoTileReader(getAutoTileDatabase());
    if (databases & AUTO_TILE_DATABASE)
        autoTileReader.execThreaded(projectPath + AUTO_TILE_DATABASE_PATH, "AutoTileDatabase");

    // sprite database
    SpriteDatabaseXMLReader spriteReader(getSpriteDatabase());
    if (databases & SPRITE_DATABASE)
        spriteReader.execThreaded(projectPath + SPRITE_DATABASE_PATH, "SpriteDatabase");

    // animation database
    AnimationDatabaseXMLReader animationReader(getAnimationDatabase());
    if (databases & ANIMATION_DATABASE)
        animationReader.execThreaded(projectPath + ANIMATION_DATABASE_PATH, "AnimationDatabase");

    // load map database
    MapDatabaseXMLReader mapReader(getMapDatabase());
    if (databases & MAP_DATABASE)
        mapReader.execThreaded(projectPath + MAP_DATABASE_PATH, "MapDatabase");

    // load text database
    LocalsDatabaseXMLReader localsReader(getLocalsDatabase());
    if (databases & LOCALS_DATABASE)
        localsReader.execThreaded(projectPath + LOCALS_DATABASE_PATH, "LocalsDatabase");

    // load worldobject database
    WorldObjectDatabaseXMLReader worldObjectReader(getWorldObjectDatabase());
    if (databases & WORLD_OBJECT_DATABASE)
        worldObjectReader.execThreaded(projectPath + WORLD_OBJECT_DATABASE_PATH, "WorldObjectDatabase");

    // load object animation type database
    ObjectAnimationTypeDatabaseXMLReader objectAnimationTypeReader(getObjectAnimationTypeDatabase());
    if (databases & OBJECT_ANIMATION_TYPE_DATABASE)
        objectAnimationTypeReader.execThreaded(projectPath + OBJECT_ANIMATION_TYPE_DATABASE_PATH, "ObjectAnimationTypeDatabase");

    // ToDo: add load here

    bool success = true;
    success &= tileReader.waitForSuccess();
    success &= tileSetReader.waitForSuccess();
    success &= autoTileReader.waitForSuccess();
    success &= spriteReader.waitForSuccess();
    success &= animationReader.waitForSuccess();
    success &= mapReader.waitForSuccess();
    success &= localsReader.waitForSuccess();
    success &= worldObjectReader.waitForSuccess();
    success &= objectAnimationTypeReader.waitForSuccess();
    //return success;
    return true;
}

bool DatabaseMgr::saveDatabase(const QString &projectPath, uint32 databases)
{
    QDir projectDir(projectPath);
    if (!projectDir.exists())
        return false;

    // TileDB
    TileDatabaseXMLWriter tileDBWriter(getTileDatabase());
    if (databases & TILE_DATABASE)
        tileDBWriter.execThreaded(projectPath + TILE_DATABASE_PATH, "TileDatabase");

    // TileSetDB
    TileSetDatabaseXMLWriter tileSetDBWriter(getTileSetDatabase());
    if (databases & TILE_SET_DATABASE)
        tileSetDBWriter.execThreaded(projectPath + TILE_SET_DATABASE_PATH, "TileSetDatabase");

    // AutoTileDB
    AutoTileDatabaseXMLWriter autoTileDBWriter(getAutoTileDatabase());
    if (databases & AUTO_TILE_DATABASE)
        autoTileDBWriter.execThreaded(projectPath + AUTO_TILE_DATABASE_PATH, "AutoTileDatabase");

    // SpriteDB
    SpriteDatabaseXMLWriter spriteDBWriter(getSpriteDatabase());
    if (databases & SPRITE_DATABASE)
        spriteDBWriter.execThreaded(projectPath + SPRITE_DATABASE_PATH, "SpriteDatabase");

    // AnimationDB
    AnimationDatabaseXMLWriter animationDBWriter(getAnimationDatabase());
    if (databases & ANIMATION_DATABASE)
        animationDBWriter.execThreaded(projectPath + ANIMATION_DATABASE_PATH, "AnimationDatabase");

    // LocalsDB
    LocalsDatabaseXMLWriter localsDBWriter(getLocalsDatabase());
    if (databases & LOCALS_DATABASE)
        localsDBWriter.execThreaded(projectPath + LOCALS_DATABASE_PATH, "LocalsDatabase");

    // WorldObjectDB
    WorldObjectDatabaseXMLWriter worldObjectDBWriter(getWorldObjectDatabase());
    if (databases & WORLD_OBJECT_DATABASE)
        worldObjectDBWriter.execThreaded(projectPath + WORLD_OBJECT_DATABASE_PATH, "WorldObjectDatabase");

    // ObjectAnimationDB
    ObjectAnimationTypeDatabaseXMLWriter objectAnimationDBWriter(getObjectAnimationTypeDatabase());
    if (databases & OBJECT_ANIMATION_TYPE_DATABASE)
        objectAnimationDBWriter.execThreaded(projectPath + OBJECT_ANIMATION_TYPE_DATABASE_PATH, "ObjectAnimationTypeDatabase");

    // MapDB
    MapDatabaseXMLWriter mapDBWriter(getMapDatabase());
    if (databases & MAP_DATABASE)
        mapDBWriter.execThreaded(projectPath + MAP_DATABASE_PATH, "MapDatabase");

    // ToDo: save changes

    bool success = true;
    success &= tileDBWriter.waitForSuccess();
    success &= tileSetDBWriter.waitForSuccess();
    success &= autoTileDBWriter.waitForSuccess();
    success &= spriteDBWriter.waitForSuccess();
    success &= animationDBWriter.waitForSuccess();
    success &= localsDBWriter.waitForSuccess();
    success &= worldObjectDBWriter.waitForSuccess();
    success &= objectAnimationDBWriter.waitForSuccess();
    success &= mapDBWriter.waitForSuccess();
    return success;
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
    return std::move(pixmap);
}