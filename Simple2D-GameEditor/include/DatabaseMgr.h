#ifndef DATABASE_MGR_H
#define DATABASE_MGR_H

#include "Database.h"
#include "MapDatabase.h"

namespace DATABASE
{
    // database store path
    const QString TILE_DATABASE_PATH = "/Game/TileDatabase.xml";
    const QString TILE_SET_DATABASE_PATH = "/Game/TileSetDatabase.xml";
    const QString AUTO_TILE_DATABASE_PATH = "/Game/AutoTileDatabase.xml";
    const QString SPRITE_DATABASE_PATH = "/Game/SpriteDatabase.xml";
    const QString ANIMATION_DATABASE_PATH = "/Game/AnimationDatabase.xml";
    const QString WORLD_OBJECT_DATABASE_PATH = "/Game/WorldObjectDatabase.xml";
    const QString DYNAMIC_OBJECT_DATABASE_PATH = "/Game/DynamicObjectDatabase.xml";
    const QString OBJECT_ANIMATION_TYPE_DATABASE_PATH = "/Game/ObjectAnimationTypeDatabase.xml";
    const QString MAP_DATABASE_PATH = "/Game/MapDatabase.xml";
    const QString LOCALS_DATABASE_PATH = "/Game/LocalsDatabase.xml";

    enum DatabaseType
    {
        TILE_DATABASE           = 0x0001,
        TILE_SET_DATABASE       = 0x0002,
        AUTO_TILE_DATABASE      = 0x0004,
        SPRITE_DATABASE         = 0x0008,
        ANIMATION_DATABASE      = 0x0010,
        OBJECT_ANIMATION_TYPE_DATABASE  = 0x0020,

        WORLD_OBJECT_DATABASE   = 0x0040,
        DYNAMIC_OBJECT_DATABASE = 0x0080,

        MAP_DATABASE            = 0x0100,

        LOCALS_DATABASE         = 0x0200,

        ALL_DATABASES           = TILE_DATABASE | TILE_SET_DATABASE | AUTO_TILE_DATABASE | SPRITE_DATABASE | ANIMATION_DATABASE | OBJECT_ANIMATION_TYPE_DATABASE |
                                    WORLD_OBJECT_DATABASE | DYNAMIC_OBJECT_DATABASE | MAP_DATABASE | LOCALS_DATABASE
    };

    class DatabaseMgr
    {
    public:
        DatabaseMgr();

        inline ConstTileDatabasePtr getTileDatabase() const { return m_pTileDatabase; }
        inline TileDatabasePtr getTileDatabase() { return m_pTileDatabase; }
        inline void setTileDatabase(const TileDatabasePtr &db) { *m_pTileDatabase = *db; }

        inline ConstTileSetDatabasePtr getTileSetDatabase() const { return m_pTileSetDatabase; }
        inline TileSetDatabasePtr getTileSetDatabase() { return m_pTileSetDatabase; }
        inline void setTileSetDatabase(const TileSetDatabasePtr &db) { *m_pTileSetDatabase = *db; }

        inline ConstAutoTileDatabasePtr getAutoTileDatabase() const { return m_pAutoTileDatabase; }
        inline AutoTileDatabasePtr getAutoTileDatabase() { return m_pAutoTileDatabase; }
        inline void setAutoTileDatabase(const AutoTileDatabasePtr &db) { *m_pAutoTileDatabase = *db; }

        inline SpriteDatabasePtr getSpriteDatabase() { return m_pSpriteDatabase; }
        inline ConstSpriteDatabasePtr getSpriteDatabase() const { return m_pSpriteDatabase; }
        inline void setSpriteDatabase(const SpriteDatabasePtr &db) { *m_pSpriteDatabase = *db; }

        inline AnimationDatabasePtr getAnimationDatabase() { return m_pAnimationDatabase; }
        inline ConstAnimationDatabasePtr getAnimationDatabase() const { return m_pAnimationDatabase; }
        inline void setAnimationDatabase(const AnimationDatabasePtr &db) { *m_pAnimationDatabase = *db; }

        inline WorldObjectDatabasePtr getWorldObjectDatabase() { return m_pWorldObjectDatabase; }
        inline ConstWorldObjectDatabasePtr getWorldObjectDatabase() const { return m_pWorldObjectDatabase; }
        inline void setWorldObjectDatabase(const WorldObjectDatabasePtr &db) { *m_pWorldObjectDatabase = *db; }

        inline DynamicObjectDatabasePtr getDynamicObjectDatabase() { return m_pDynamicObjectDatabase; }
        inline ConstDynamicObjectDatabasePtr getDynamicObjectDatabase() const { return m_pDynamicObjectDatabase; }
        inline void setDynamicObjectDatabase(const DynamicObjectDatabasePtr &db) { *m_pDynamicObjectDatabase = *db; }

        inline ObjectAnimationTypeDatabasePtr getObjectAnimationTypeDatabase() { return m_pObjectAnimationTypeDatabase; }
        inline ConstObjectAnimationTypeDatabasePtr getObjectAnimationTypeDatabase() const { return m_pObjectAnimationTypeDatabase; }
        inline void setObjectAnimationTypeDatabase(const ObjectAnimationTypeDatabasePtr &db) { *m_pObjectAnimationTypeDatabase = *db; }

        inline MapDatabasePtr getMapDatabase() { return m_pMapDatabase; }
        inline ConstMapDatabasePtr getMapDatabase() const { return m_pMapDatabase; }
        inline void setMapDatabase(const MapDatabasePtr &db) { *m_pMapDatabase = *db; }

        inline LocalsDatabasePtr getLocalsDatabase() { return m_pLocalsDatabase; }
        inline ConstLocalsDatabasePtr getLocalsDatabase() const { return m_pLocalsDatabase; }
        inline void setTextDatabase(const LocalsDatabasePtr &db) { *m_pLocalsDatabase = *db; }

        virtual void clear();

        // in/output
        virtual bool loadDatabase(const QString &projectPath, uint32 databases = ALL_DATABASES);
        virtual bool saveDatabase(const QString &projectPath, uint32 databases = ALL_DATABASES);

    private:
        TileDatabasePtr m_pTileDatabase;
        TileSetDatabasePtr m_pTileSetDatabase;
        AutoTileDatabasePtr m_pAutoTileDatabase;
        SpriteDatabasePtr m_pSpriteDatabase;
        AnimationDatabasePtr m_pAnimationDatabase;
        WorldObjectDatabasePtr m_pWorldObjectDatabase;
        DynamicObjectDatabasePtr m_pDynamicObjectDatabase;
        ObjectAnimationTypeDatabasePtr m_pObjectAnimationTypeDatabase;
        MapDatabasePtr m_pMapDatabase;
        LocalsDatabasePtr m_pLocalsDatabase;
    };
    typedef std::shared_ptr<DatabaseMgr> DatabaseMgrPtr;
    typedef std::shared_ptr<const DatabaseMgr> ConstDatabaseMgrPtr;

    namespace TILE_SET
    {
        QPixmap createTileSetPixmap(const QString &path, ConstTileSetPrototypePtr proto, ConstTileDatabasePtr tileDB);
    }
}
#endif