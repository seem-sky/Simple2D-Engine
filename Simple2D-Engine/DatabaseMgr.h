#ifndef DATABASE_MGR_H
#define DATABASE_MGR_H

#include "DatabasePrototypes.h"
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

    class ObjectAnimationTypeDatabase : public Database<ObjectAnimationTypePrototype>
    {
    public:
        ObjectAnimationTypeDatabase() : Database()
        {
            clear();
        }

        void clear()
        {
            Database::clear();
            Database::setItem(MAP::DIRECTION_UP+1, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(MAP::DIRECTION_UP+1, "STAND_UP")));
            Database::setItem(MAP::DIRECTION_RIGHT+1, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(MAP::DIRECTION_RIGHT+1, "STAND_RIGHT")));
            Database::setItem(MAP::DIRECTION_DOWN+1, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(MAP::DIRECTION_DOWN+1, "STAND_DOWN")));
            Database::setItem(MAP::DIRECTION_LEFT+1, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(MAP::DIRECTION_LEFT+1, "STAND_LEFT")));
            Database::setItem(5, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(5, "WALK_UP")));
            Database::setItem(6, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(6, "WALK_RIGHT")));
            Database::setItem(7, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(7, "WALK_DOWN")));
            Database::setItem(8, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(8, "WALK_LEFT")));
        }

        void setItem(uint32 uiID, ObjectAnimationTypePrototypePtr &prototype)
        {
            if (uiID > MIN_DYNAMIC_OBJECT_POSE)
                Database::setItem(uiID, prototype);
        }
    };
    // object animation type database typedefs
    typedef std::shared_ptr<ObjectAnimationTypeDatabase> ObjectAnimationTypeDatabasePtr;
    typedef std::shared_ptr<const ObjectAnimationTypeDatabase> ConstObjectAnimationTypeDatabasePtr;

    /*#####
    # DatabaseMgr
    #####*/
    // tile database typedefs
    typedef Database<TilePrototype> TileDatabase;
    typedef std::shared_ptr<TileDatabase> TileDatabasePtr;
    typedef std::shared_ptr<const TileDatabase> ConstTileDatabasePtr;
    // tileset database typedefs
    typedef Database<TILE_SET::TileSetPrototype> TileSetDatabase;
    typedef std::shared_ptr<TileSetDatabase> TileSetDatabasePtr;
    typedef std::shared_ptr<const TileSetDatabase> ConstTileSetDatabasePtr;
    // autotile database typedefs
    typedef Database<AUTO_TILE::AutoTilePrototype> AutoTileDatabase;
    typedef std::shared_ptr<AutoTileDatabase> AutoTileDatabasePtr;
    typedef std::shared_ptr<const AutoTileDatabase> ConstAutoTileDatabasePtr;
    // sprite database typedefs
    typedef Database<SpritePrototype> SpriteDatabase;
    typedef std::shared_ptr<SpriteDatabase> SpriteDatabasePtr;
    typedef std::shared_ptr<const SpriteDatabase> ConstSpriteDatabasePtr;
    // animation database typedefs
    typedef Database<AnimationPrototype> AnimationDatabase;
    typedef std::shared_ptr<AnimationDatabase> AnimationDatabasePtr;
    typedef std::shared_ptr<const AnimationDatabase> ConstAnimationDatabasePtr;
    // world object database typedefs
    typedef Database<WorldObjectPrototype> WorldObjectDatabase;
    typedef std::shared_ptr<WorldObjectDatabase> WorldObjectDatabasePtr;
    typedef std::shared_ptr<const WorldObjectDatabase> ConstWorldObjectDatabasePtr;
    // dynamic object database typedefs
    typedef Database<DynamicObjectPrototype> DynamicObjectDatabase;
    typedef std::shared_ptr<DynamicObjectDatabase> DynamicObjectDatabasePtr;
    typedef std::shared_ptr<const DynamicObjectDatabase> ConstDynamicObjectDatabasePtr;
    // text database typedefs
    typedef Database<LocalisationPrototype> LocalsDatabase;
    typedef std::shared_ptr<LocalsDatabase> LocalsDatabasePtr;
    typedef std::shared_ptr<const LocalsDatabase> ConstLocalsDatabasePtr;
    // map database typedefs
    typedef std::shared_ptr<MAP::MapDatabase> MapDatabasePtr;
    typedef std::shared_ptr<const MAP::MapDatabase> ConstMapDatabasePtr;
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