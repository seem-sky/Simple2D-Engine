#ifndef DATABASE_MGR_H
#define DATABASE_MGR_H

#include "MapDatabase.h"
#include <QtGui/QPixmap>

namespace DATABASE
{
    enum class DatabaseType
    {
        TILE_DATABASE,
        TILE_SET_DATABASE,
        AUTO_TILE_DATABASE,

        SPRITE_DATABASE,
        ANIMATION_DATABASE,
        ANIMATION_TYPE_DATABASE,

        WORLD_OBJECT_DATABASE,

        LOCALISATION_DATABASE,

        MAP_DATABASE
    };
    const uint32 DATABASE_COUNT = 9;

    // database store path
    const std::array<const char*, DATABASE_COUNT> DATABASE_FILE =
    {
        "/Game/TileDatabase.xml",
        "/Game/TileSetDatabase.xml",
        "/Game/AutoTileDatabase.xml",
        "/Game/SpriteDatabase.xml",
        "/Game/AnimationDatabase.xml",
        "/Game/ObjectAnimationTypeDatabase.xml",
        "/Game/WorldObjectDatabase.xml",
        "/Game/LocalsDatabase.xml",
        "/Game/MapDatabase.xml"
    };

    class DatabaseMgr
    {
    public:
        enum DatabaseTypeFlag
        {
            TILE_DATABASE           = 0x0001,
            TILE_SET_DATABASE       = 0x0002,
            AUTO_TILE_DATABASE      = 0x0004,

            SPRITE_DATABASE         = 0x0008,
            ANIMATION_DATABASE      = 0x0010,
            ANIMATION_TYPE_DATABASE  = 0x0020,

            WORLD_OBJECT_DATABASE   = 0x0040,

            LOCALISATION_DATABASE   = 0x0080,

            MAP_DATABASE            = 0x0100,

            ALL_DATABASES           = TILE_DATABASE | TILE_SET_DATABASE | AUTO_TILE_DATABASE | SPRITE_DATABASE | ANIMATION_DATABASE | ANIMATION_TYPE_DATABASE |
            WORLD_OBJECT_DATABASE | MAP_DATABASE | LOCALISATION_DATABASE
        };

    public:
        DatabaseMgr();

        // overwrite copy constructor
        DatabaseMgr(const DatabaseMgr& other);

        const TileDatabase* getTileDatabase() const { return dynamic_cast<TileDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::TILE_DATABASE)).get()); }
        TileDatabase* getTileDatabase() { return dynamic_cast<TileDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::TILE_DATABASE)).get()); }

        const TileSetDatabase* getTileSetDatabase() const { return dynamic_cast<TileSetDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::TILE_SET_DATABASE)).get()); }
        TileSetDatabase* getTileSetDatabase() { return dynamic_cast<TileSetDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::TILE_SET_DATABASE)).get()); }

        const AutoTileDatabase* getAutoTileDatabase() const { return dynamic_cast<AutoTileDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::AUTO_TILE_DATABASE)).get()); }
        AutoTileDatabase* getAutoTileDatabase() { return dynamic_cast<AutoTileDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::AUTO_TILE_DATABASE)).get()); }

        const SpriteDatabase* getSpriteDatabase() const { return dynamic_cast<SpriteDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::SPRITE_DATABASE)).get()); }
        SpriteDatabase* getSpriteDatabase() { return dynamic_cast<SpriteDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::SPRITE_DATABASE)).get()); }

        const AnimationDatabase* getAnimationDatabase() const { return dynamic_cast<AnimationDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::ANIMATION_DATABASE)).get()); }
        AnimationDatabase* getAnimationDatabase() { return dynamic_cast<AnimationDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::ANIMATION_DATABASE)).get()); }

        const AnimationTypeDatabase* getAnimationTypeDatabase() const { return dynamic_cast<AnimationTypeDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::ANIMATION_TYPE_DATABASE)).get()); }
        AnimationTypeDatabase* getAnimationTypeDatabase() { return dynamic_cast<AnimationTypeDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::ANIMATION_TYPE_DATABASE)).get()); }

        const WorldObjectDatabase* getWorldObjectDatabase() const { return dynamic_cast<WorldObjectDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::WORLD_OBJECT_DATABASE)).get()); }
        WorldObjectDatabase* getWorldObjectDatabase() { return dynamic_cast<WorldObjectDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::WORLD_OBJECT_DATABASE)).get()); }

        const LocalisationDatabase* getLocalisationDatabase() const { return dynamic_cast<LocalisationDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::LOCALISATION_DATABASE)).get()); }
        LocalisationDatabase* getLocalisationDatabase() { return dynamic_cast<LocalisationDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::LOCALISATION_DATABASE)).get()); }
        
        const MapDatabase* getMapDatabase() const { return dynamic_cast<MapDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::MAP_DATABASE)).get()); }
        MapDatabase* getMapDatabase() { return dynamic_cast<MapDatabase*>(m_Databases.at(static_cast<uint32>(DatabaseType::MAP_DATABASE)).get()); }

        void setTileDatabase(TileDatabase* db) { m_Databases.at(static_cast<uint32>(DatabaseType::TILE_DATABASE)) = std::unique_ptr<IDatabase>(db); }
        void setTileSetDatabase(TileSetDatabase* db) { m_Databases.at(static_cast<uint32>(DatabaseType::TILE_SET_DATABASE)) = std::unique_ptr<IDatabase>(db); }
        void setAutoTileDatabase(AutoTileDatabase* db) { m_Databases.at(static_cast<uint32>(DatabaseType::AUTO_TILE_DATABASE)) = std::unique_ptr<IDatabase>(db); }
        void setSpriteDatabase(SpriteDatabase* db) { m_Databases.at(static_cast<uint32>(DatabaseType::SPRITE_DATABASE)) = std::unique_ptr<IDatabase>(db); }
        void setAnimationDatabase(AnimationDatabase* db) { m_Databases.at(static_cast<uint32>(DatabaseType::ANIMATION_DATABASE)) = std::unique_ptr<IDatabase>(db); }
        void setAnimationTypeDatabase(AnimationTypeDatabase* db) { m_Databases.at(static_cast<uint32>(DatabaseType::ANIMATION_TYPE_DATABASE)) = std::unique_ptr<IDatabase>(db); }
        void setWorldObjectDatabase(WorldObjectDatabase* db) { m_Databases.at(static_cast<uint32>(DatabaseType::WORLD_OBJECT_DATABASE)) = std::unique_ptr<IDatabase>(db); }
        void setLocalisationDatabase(LocalisationDatabase* db) { m_Databases.at(static_cast<uint32>(DatabaseType::LOCALISATION_DATABASE)) = std::unique_ptr<IDatabase>(db); }
        void setMapDatabase(MapDatabase* db) { m_Databases.at(static_cast<uint32>(DatabaseType::MAP_DATABASE)) = std::unique_ptr<IDatabase>(db); }

        const IDatabase* getDatabase(DatabaseType type) const { return m_Databases.at(static_cast<uint32>(type)).get(); }
        IDatabase* getDatabase(DatabaseType type) { return m_Databases.at(static_cast<uint32>(type)).get(); }

        void clear();
        void takeFrom(DatabaseMgr& other, uint32 databases = ALL_DATABASES);
        void copyFrom(const DatabaseMgr& other, uint32 databases = ALL_DATABASES);

        // in/output
        bool loadDatabase(const QString& projectPath, uint32 databases = ALL_DATABASES);
        bool saveDatabase(const QString& projectPath, uint32 databases = ALL_DATABASES);

    private:
        std::array<std::unique_ptr<IDatabase>, DATABASE_COUNT> m_Databases;
    };

    namespace PROTOTYPE
    {
        namespace TILE_SET
        {
            QPixmap createTileSetPixmap(const QString& path, const TileSetPrototype* pSet, const TileDatabase* pTileDB);
        }
    }
}
#endif