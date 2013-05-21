#ifndef DATABASE_MGR_H
#define DATABASE_MGR_H

#include "DatabasePrototypes.h"
#include "MapDatabase.h"

namespace DATABASE
{
    // database store path
    const std::string TILE_DATABASE_PATH = "/Game/TileDatabase.xml";
    const std::string AUTO_TILE_DATABASE_PATH = "/Game/AutoTileDatabase.xml";
    const std::string SPRITE_DATABASE_PATH = "/Game/SpriteDatabase.xml";
    const std::string ANIMATION_DATABASE_PATH = "/Game/AnimationDatabase.xml";
    const std::string WORLD_OBJECT_DATABASE_PATH = "/Game/WorldObjectDatabase.xml";
    const std::string DYNAMIC_OBJECT_DATABASE_PATH = "/Game/DynamicObjectDatabase.xml";
    const std::string OBJECT_ANIMATION_TYPE_DATABASE_PATH = "/Game/ObjectAnimationTypeDatabase.xml";
    const std::string MAP_DATABASE_PATH = "/Game/MapDatabase.xml";
    const std::string LOCALS_DATABASE_PATH = "/Game/LocalsDatabase.xml";

    class ObjectAnimationTypeDatabase : public Database<ObjectAnimationTypePrototype>
    {
    public:
        ObjectAnimationTypeDatabase() : Database()
        {
            Database::setPrototype(1, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(1, "STAND_LEFT")));
            Database::setPrototype(2, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(2, "STAND_RIGHT")));
            Database::setPrototype(3, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(3, "STAND_UP")));
            Database::setPrototype(4, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(4, "STAND_DOWN")));
            Database::setPrototype(5, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(5, "WALK_LEFT")));
            Database::setPrototype(6, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(6, "WALK_RIGHT")));
            Database::setPrototype(7, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(7, "WALK_UP")));
            Database::setPrototype(8, ObjectAnimationTypePrototypePtr(new ObjectAnimationTypePrototype(8, "WALK_DOWN")));
        }

        void setPrototype(uint32 uiID, ObjectAnimationTypePrototypePtr &prototype)
        {
            if (uiID > MIN_DYNAMIC_OBJECT_POSE)
                Database::setPrototype(uiID, prototype);
        }
    };
    // object animation type database typedefs
    typedef boost::shared_ptr<ObjectAnimationTypeDatabase> ObjectAnimationTypeDatabasePtr;
    typedef boost::shared_ptr<const ObjectAnimationTypeDatabase> ConstObjectAnimationTypeDatabasePtr;

    /*#####
    # DatabaseMgr
    #####*/
    // tile database typedefs
    typedef Database<TilePrototype> TileDatabase;
    typedef boost::shared_ptr<TileDatabase> TileDatabasePtr;
    typedef boost::shared_ptr<const TileDatabase> ConstTileDatabasePtr;
    // autotile database typedefs
    typedef Database<AutoTilePrototype> AutoTileDatabase;
    typedef boost::shared_ptr<AutoTileDatabase> AutoTileDatabasePtr;
    typedef boost::shared_ptr<const AutoTileDatabase> ConstAutoTileDatabasePtr;
    // sprite database typedefs
    typedef Database<SpritePrototype> SpriteDatabase;
    typedef boost::shared_ptr<SpriteDatabase> SpriteDatabasePtr;
    typedef boost::shared_ptr<const SpriteDatabase> ConstSpriteDatabasePtr;
    // animation database typedefs
    typedef Database<AnimationPrototype> AnimationDatabase;
    typedef boost::shared_ptr<AnimationDatabase> AnimationDatabasePtr;
    typedef boost::shared_ptr<const AnimationDatabase> ConstAnimationDatabasePtr;
    // world object database typedefs
    typedef Database<WorldObjectPrototype> WorldObjectDatabase;
    typedef boost::shared_ptr<WorldObjectDatabase> WorldObjectDatabasePtr;
    typedef boost::shared_ptr<const WorldObjectDatabase> ConstWorldObjectDatabasePtr;
    // dynamic object database typedefs
    typedef Database<DynamicObjectPrototype> DynamicObjectDatabase;
    typedef boost::shared_ptr<DynamicObjectDatabase> DynamicObjectDatabasePtr;
    typedef boost::shared_ptr<const DynamicObjectDatabase> ConstDynamicObjectDatabasePtr;
    // text database typedefs
    typedef Database<TextPrototype> TextDatabase;
    typedef boost::shared_ptr<TextDatabase> TextDatabasePtr;
    typedef boost::shared_ptr<const TextDatabase> ConstTextDatabasePtr;
    // map database typedefs
    typedef boost::shared_ptr<MAP::MapDatabase> MapDatabasePtr;
    typedef boost::shared_ptr<const MAP::MapDatabase> ConstMapDatabasePtr;
    class DatabaseMgr
    {
    public:
        inline ConstTileDatabasePtr getTileDatabase() const { return m_pTileDatabase; }
        inline TileDatabasePtr getTileDatabase() { return m_pTileDatabase; }
        inline void setTileDatabase(const TileDatabasePtr &db) { *m_pTileDatabase.get() = *db; }

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

        inline TextDatabasePtr getTextDatabase() { return m_pTextDatabase; }
        inline ConstTextDatabasePtr getTextDatabase() const { return m_pTextDatabase; }
        inline void setTextDatabase(const TextDatabasePtr &db) { *m_pTextDatabase = *db; }

        virtual void clear()
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

        DatabaseMgr()
        {
            m_pTileDatabase = TileDatabasePtr(new TileDatabase());
            m_pAutoTileDatabase = AutoTileDatabasePtr(new AutoTileDatabase());
            m_pSpriteDatabase = SpriteDatabasePtr(new SpriteDatabase());
            m_pAnimationDatabase = AnimationDatabasePtr(new AnimationDatabase());
            m_pWorldObjectDatabase = WorldObjectDatabasePtr(new WorldObjectDatabase());
            m_pDynamicObjectDatabase = DynamicObjectDatabasePtr(new DynamicObjectDatabase());
            m_pObjectAnimationTypeDatabase = ObjectAnimationTypeDatabasePtr(new ObjectAnimationTypeDatabase());
            m_pMapDatabase = MapDatabasePtr(new MAP::MapDatabase());
            m_pTextDatabase = TextDatabasePtr(new TextDatabase());
        }

    private:
        TileDatabasePtr m_pTileDatabase;
        AutoTileDatabasePtr m_pAutoTileDatabase;
        SpriteDatabasePtr m_pSpriteDatabase;
        AnimationDatabasePtr m_pAnimationDatabase;
        WorldObjectDatabasePtr m_pWorldObjectDatabase;
        DynamicObjectDatabasePtr m_pDynamicObjectDatabase;
        ObjectAnimationTypeDatabasePtr m_pObjectAnimationTypeDatabase;
        MapDatabasePtr m_pMapDatabase;
        TextDatabasePtr m_pTextDatabase;
    };
    typedef boost::shared_ptr<DatabaseMgr> DatabaseMgrPtr;
    typedef boost::shared_ptr<const DatabaseMgr> ConstDatabaseMgrPtr;
}
#endif