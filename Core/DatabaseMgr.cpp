#include "DatabaseMgr.h"
#include <QtCore/QDir>
#include "DatabaseIO.h"
#include <boost/thread.hpp>

using namespace DATABASE;

DatabaseMgr::DatabaseMgr()
{
    m_pTileDatabase = TileDatabasePtr(new TileDatabase());
    m_pAutoTileDatabase = AutoTileDatabasePtr(new AutoTileDatabase());
    m_pSpriteDatabase = SpriteDatabasePtr(new SpriteDatabase());
    m_pAnimationDatabase = AnimationDatabasePtr(new AnimationDatabase());
    m_pWorldObjectDatabase = WorldObjectDatabasePtr(new WorldObjectDatabase());
    m_pDynamicObjectDatabase = DynamicObjectDatabasePtr(new DynamicObjectDatabase());
    m_pObjectAnimationTypeDatabase = ObjectAnimationTypeDatabasePtr(new ObjectAnimationTypeDatabase());
    m_pMapDatabase = MapDatabasePtr(new MAP::MapDatabase());
    m_pTextDatabase = LocalsDatabasePtr(new LocalsDatabase());
}

void DatabaseMgr::clear()
{
    if (m_pTileDatabase) m_pTileDatabase->clear();
    if (m_pAutoTileDatabase) m_pAutoTileDatabase->clear();
    if (m_pSpriteDatabase) m_pSpriteDatabase->clear();
    if (m_pAnimationDatabase) m_pAnimationDatabase->clear();
    if (m_pWorldObjectDatabase) m_pWorldObjectDatabase->clear();
    if (m_pDynamicObjectDatabase) m_pDynamicObjectDatabase->clear();
    if (m_pObjectAnimationTypeDatabase) m_pObjectAnimationTypeDatabase->clear();
    if (m_pMapDatabase) m_pMapDatabase->clear();
    if (m_pTextDatabase) m_pTextDatabase->clear();
}

bool DatabaseMgr::loadDatabase(const QString &projectPath)
{
    clear();
    QDir projectDir(projectPath);
    if (!projectDir.exists())
        return false;
    // tile database
    TileDatabaseXMLReader tileReader(getTileDatabase());
    tileReader.execThreaded(projectPath + TILE_DATABASE_PATH, "TileDatabase");

    // autotile database
    AutoTileDatabaseXMLReader autoTileReader(getAutoTileDatabase());
    autoTileReader.execThreaded(projectPath + AUTO_TILE_DATABASE_PATH, "AutoTileDatabase");

    // sprite database
    SpriteDatabaseXMLReader spriteReader(getSpriteDatabase());
    spriteReader.execThreaded(projectPath + SPRITE_DATABASE_PATH, "SpriteDatabase");

    // animation database
    AnimationDatabaseXMLReader animationReader(getAnimationDatabase());
    animationReader.execThreaded(projectPath + ANIMATION_DATABASE_PATH, "AnimationDatabase");

    // load map database
    MapDatabaseXMLReader mapReader(getMapDatabase());
    mapReader.execThreaded(projectPath + MAP_DATABASE_PATH, "MapDatabase");

    // load text database
    LocalsDatabaseXMLReader localsReader(getLocalsDatabase());
    localsReader.execThreaded(projectPath + LOCALS_DATABASE_PATH, "LocalsDatabase");

    // load worldobject database
    WorldObjectDatabaseXMLReader worldObjectReader(getWorldObjectDatabase());
    worldObjectReader.execThreaded(projectPath + WORLD_OBJECT_DATABASE_PATH, "WorldObjectDatabase");

    // load object animation type database
    ObjectAnimationTypeDatabaseXMLReader objectAnimationTypeReader(getObjectAnimationTypeDatabase());
    objectAnimationTypeReader.execThreaded(projectPath + OBJECT_ANIMATION_TYPE_DATABASE_PATH, "ObjectAnimationTypeDatabase");

    // ToDo: add load here

    bool success = true;
    success &= tileReader.waitForSuccess();
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

bool DatabaseMgr::saveDatabase(const QString &projectPath)
{
    QDir projectDir(projectPath);
    if (!projectDir.exists())
        return false;

    // TileDB
    TileDatabaseXMLWriter tileDBWriter(getTileDatabase());
    tileDBWriter.execThreaded(projectPath + TILE_DATABASE_PATH, "TileDatabase");

    // AutoTileDB
    AutoTileDatabaseXMLWriter autoTileDBWriter(getAutoTileDatabase());
    autoTileDBWriter.execThreaded(projectPath + AUTO_TILE_DATABASE_PATH, "AutoTileDatabase");

    // SpriteDB
    SpriteDatabaseXMLWriter spriteDBWriter(getSpriteDatabase());
    spriteDBWriter.execThreaded(projectPath + SPRITE_DATABASE_PATH, "SpriteDatabase");

    // AnimationDB
    AnimationDatabaseXMLWriter animationDBWriter(getAnimationDatabase());
    animationDBWriter.execThreaded(projectPath + ANIMATION_DATABASE_PATH, "AnimationDatabase");

    // LocalsDB
    LocalsDatabaseXMLWriter localsDBWriter(getLocalsDatabase());
    localsDBWriter.execThreaded(projectPath + LOCALS_DATABASE_PATH, "LocalsDatabase");

    // WorldObjectDB
    WorldObjectDatabaseXMLWriter worldObjectDBWriter(getWorldObjectDatabase());
    worldObjectDBWriter.execThreaded(projectPath + WORLD_OBJECT_DATABASE_PATH, "WorldObjectDatabase");

    // ObjectAnimationDB
    ObjectAnimationTypeDatabaseXMLWriter objectAnimationDBWriter(getObjectAnimationTypeDatabase());
    objectAnimationDBWriter.execThreaded(projectPath + OBJECT_ANIMATION_TYPE_DATABASE_PATH, "ObjectAnimationTypeDatabase");

    // ObjectAnimationDB
    MapDatabaseXMLWriter mapDBWriter(getMapDatabase());
    mapDBWriter.execThreaded(projectPath + MAP_DATABASE_PATH, "MapDatabase");

    // ToDo: save changes

    bool success = true;
    success &= tileDBWriter.waitForSuccess();
    success &= autoTileDBWriter.waitForSuccess();
    success &= spriteDBWriter.waitForSuccess();
    success &= animationDBWriter.waitForSuccess();
    success &= localsDBWriter.waitForSuccess();
    success &= worldObjectDBWriter.waitForSuccess();
    success &= objectAnimationDBWriter.waitForSuccess();
    success &= mapDBWriter.waitForSuccess();
    return success;
}