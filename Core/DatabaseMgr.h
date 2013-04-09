#ifndef DATABASE_MGR_H
#define DATABASE_MGR_H

#include "DatabasePrototypes.h"
#include "MapDatabase.h"

namespace DATABASE
{
    // database store path
    const std::string TILE_DATABASE_PATH = "/Game/GameDatabase.xml";
    const std::string AUTO_TILE_DATABASE_PATH = "/Game/GameDatabase.xml";
    const std::string SPRITE_DATABASE_PATH = "/Game/GameDatabase.xml";
    const std::string WORLD_OBJECT_DATABASE_PATH = "/Game/GameDatabase.xml";
    const std::string DYNAMIC_OBJECT_DATABASE_PATH = "/Game/GameDatabase.xml";
    const std::string MAP_DATABASE_PATH = "/Game/MapDatabase.xml";
    const std::string TEXT_DATABASE_PATH = "/Game/GameDatabase.xml";

    /*#####
    # DatabaseMgr
    #####*/
    // tile database typedefs
    typedef Database<TilePrototype> TileDatabase;
    typedef boost::shared_ptr<TileDatabase> TileDatabasePtr;
    typedef boost::shared_ptr<const TileDatabase> ConstTileDatabasePtr;
    // tile database typedefs
    typedef Database<AutoTilePrototype> AutoTileDatabase;
    typedef boost::shared_ptr<AutoTileDatabase> AutoTileDatabasePtr;
    typedef boost::shared_ptr<const AutoTileDatabase> ConstAutoTileDatabasePtr;
    // objectTexture database typedefs
    typedef Database<SpritePrototype> SpriteDatabase;
    typedef boost::shared_ptr<SpriteDatabase> SpriteDatabasePtr;
    typedef boost::shared_ptr<const SpriteDatabase> ConstSpriteDatabasePtr;
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

        inline WorldObjectDatabasePtr getWorldObjectDatabase() { return m_pWorldObjectDatabase; }
        inline ConstWorldObjectDatabasePtr getWorldObjectDatabase() const { return m_pWorldObjectDatabase; }
        inline void setWorldObjectDatabase(const WorldObjectDatabasePtr &db) { *m_pWorldObjectDatabase = *db; }

        inline DynamicObjectDatabasePtr getDynamicObjectDatabase() { return m_pDynamicObjectDatabase; }
        inline ConstDynamicObjectDatabasePtr getDynamicObjectDatabase() const { return m_pDynamicObjectDatabase; }
        inline void setDynamicObjectDatabase(const DynamicObjectDatabasePtr &db) { *m_pDynamicObjectDatabase = *db; }

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
            if (m_pWorldObjectDatabase) m_pWorldObjectDatabase->clear();
            if (m_pDynamicObjectDatabase) m_pDynamicObjectDatabase->clear();
            if (m_pMapDatabase) m_pMapDatabase->clear();
            if (m_pTextDatabase) m_pTextDatabase->clear();
        }

        DatabaseMgr()
        {
            m_pTileDatabase = TileDatabasePtr(new TileDatabase());
            m_pAutoTileDatabase = AutoTileDatabasePtr(new AutoTileDatabase());
            m_pSpriteDatabase = SpriteDatabasePtr(new SpriteDatabase());
            m_pWorldObjectDatabase = WorldObjectDatabasePtr(new WorldObjectDatabase());
            m_pDynamicObjectDatabase = DynamicObjectDatabasePtr(new DynamicObjectDatabase());
            m_pMapDatabase = MapDatabasePtr(new MAP::MapDatabase());
            m_pTextDatabase = TextDatabasePtr(new TextDatabase());
        }

    private:
        TileDatabasePtr m_pTileDatabase;
        AutoTileDatabasePtr m_pAutoTileDatabase;
        SpriteDatabasePtr m_pSpriteDatabase;
        WorldObjectDatabasePtr m_pWorldObjectDatabase;
        DynamicObjectDatabasePtr m_pDynamicObjectDatabase;
        MapDatabasePtr m_pMapDatabase;
        TextDatabasePtr m_pTextDatabase;
    };
    typedef boost::shared_ptr<DatabaseMgr> DatabaseMgrPtr;
    typedef boost::shared_ptr<const DatabaseMgr> ConstDatabaseMgrPtr;
}
#endif