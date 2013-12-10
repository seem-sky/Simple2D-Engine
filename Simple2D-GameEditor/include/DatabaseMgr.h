#ifndef DATABASE_MGR_H
#define DATABASE_MGR_H

#include "MapDatabase.h"
#include <QtGui/QPixmap>

namespace DATABASE
{
    enum DatabaseType
    {
        TILE_DATABASE,
        TILE_SET_DATABASE,
        AUTO_TILE_DATABASE,

        SPRITE_DATABASE,
        ANIMATION_DATABASE,
        ANIMATION_TYPE_DATABASE,

        WORLD_OBJECT_DATABASE,
        DYNAMIC_OBJECT_DATABASE,

        LOCALISATION_DATABASE,

        MAP_DATABASE
    };
    const uint32 DATABASE_COUNT = 10;

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
        "/Game/DynamicObjectDatabase.xml",
        "/Game/LocalsDatabase.xml",
        "/Game/MapDatabase.xml"
    };

    class DatabaseMgr
    {
    public:
        enum DatabaseType
        {
            TILE_DATABASE           = 0x0001,
            TILE_SET_DATABASE       = 0x0002,
            AUTO_TILE_DATABASE      = 0x0004,
            SPRITE_DATABASE         = 0x0008,
            ANIMATION_DATABASE      = 0x0010,
            ANIMATION_TYPE_DATABASE  = 0x0020,

            WORLD_OBJECT_DATABASE   = 0x0040,
            DYNAMIC_OBJECT_DATABASE = 0x0080,

            MAP_DATABASE            = 0x0100,

            LOCALS_DATABASE         = 0x0200,

            ALL_DATABASES           = TILE_DATABASE | TILE_SET_DATABASE | AUTO_TILE_DATABASE | SPRITE_DATABASE | ANIMATION_DATABASE | ANIMATION_TYPE_DATABASE |
            WORLD_OBJECT_DATABASE | DYNAMIC_OBJECT_DATABASE | MAP_DATABASE | LOCALS_DATABASE
        };
    public:
        DatabaseMgr();

        inline const TileDatabase* getTileDatabase() const { return dynamic_cast<TileDatabase*>(m_Databases.at(DATABASE::TILE_DATABASE).get()); }
        inline TileDatabase* getTileDatabase() { return dynamic_cast<TileDatabase*>(m_Databases.at(DATABASE::TILE_DATABASE).get()); }

        inline const TileSetDatabase* getTileSetDatabase() const { return dynamic_cast<TileSetDatabase*>(m_Databases.at(DATABASE::TILE_SET_DATABASE).get()); }
        inline TileSetDatabase* getTileSetDatabase() { return dynamic_cast<TileSetDatabase*>(m_Databases.at(DATABASE::TILE_SET_DATABASE).get()); }

        inline const AutoTileDatabase* getAutoTileDatabase() const { return dynamic_cast<AutoTileDatabase*>(m_Databases.at(DATABASE::AUTO_TILE_DATABASE).get()); }
        inline AutoTileDatabase* getAutoTileDatabase() { return dynamic_cast<AutoTileDatabase*>(m_Databases.at(DATABASE::AUTO_TILE_DATABASE).get()); }

        inline const SpriteDatabase* getSpriteDatabase() const { return dynamic_cast<SpriteDatabase*>(m_Databases.at(DATABASE::SPRITE_DATABASE).get()); }
        inline SpriteDatabase* getSpriteDatabase() { return dynamic_cast<SpriteDatabase*>(m_Databases.at(DATABASE::SPRITE_DATABASE).get()); }

        inline const AnimationDatabase* getAnimationDatabase() const { return dynamic_cast<AnimationDatabase*>(m_Databases.at(DATABASE::ANIMATION_DATABASE).get()); }
        inline AnimationDatabase* getAnimationDatabase() { return dynamic_cast<AnimationDatabase*>(m_Databases.at(DATABASE::ANIMATION_DATABASE).get()); }

        inline const AnimationTypeDatabase* getAnimationTypeDatabase() const { return dynamic_cast<AnimationTypeDatabase*>(m_Databases.at(DATABASE::ANIMATION_TYPE_DATABASE).get()); }
        inline AnimationTypeDatabase* getAnimationTypeDatabase() { return dynamic_cast<AnimationTypeDatabase*>(m_Databases.at(DATABASE::ANIMATION_TYPE_DATABASE).get()); }

        inline const WorldObjectDatabase* getWorldObjectDatabase() const { return dynamic_cast<WorldObjectDatabase*>(m_Databases.at(DATABASE::WORLD_OBJECT_DATABASE).get()); }
        inline WorldObjectDatabase* getWorldObjectDatabase() { return dynamic_cast<WorldObjectDatabase*>(m_Databases.at(DATABASE::WORLD_OBJECT_DATABASE).get()); }

        inline const DynamicObjectDatabase* getDynamicObjectDatabase() const { return dynamic_cast<DynamicObjectDatabase*>(m_Databases.at(DATABASE::DYNAMIC_OBJECT_DATABASE).get()); }
        inline DynamicObjectDatabase* getDynamicObjectDatabase() { return dynamic_cast<DynamicObjectDatabase*>(m_Databases.at(DATABASE::DYNAMIC_OBJECT_DATABASE).get()); }

        inline const LocalisationDatabase* getLocalisationDatabase() const { return dynamic_cast<LocalisationDatabase*>(m_Databases.at(DATABASE::LOCALISATION_DATABASE).get()); }
        inline LocalisationDatabase* getLocalisationDatabase() { return dynamic_cast<LocalisationDatabase*>(m_Databases.at(DATABASE::LOCALISATION_DATABASE).get()); }
        
        inline const MAP_STRUCTURE::MapDatabase* getMapDatabase() const { return dynamic_cast<MAP_STRUCTURE::MapDatabase*>(m_Databases.at(DATABASE::MAP_DATABASE).get()); }
        inline MAP_STRUCTURE::MapDatabase* getMapDatabase() { return dynamic_cast<MAP_STRUCTURE::MapDatabase*>(m_Databases.at(DATABASE::MAP_DATABASE).get()); }

        inline void setTileDatabase(TileDatabase *db) { m_Databases.at(DATABASE::TILE_DATABASE) = std::unique_ptr<IDatabase>(db); }
        inline void setTileSetDatabase(TileSetDatabase *db) { m_Databases.at(DATABASE::TILE_SET_DATABASE) = std::unique_ptr<IDatabase>(db); }
        inline void setAutoTileDatabase(AutoTileDatabase *db) { m_Databases.at(DATABASE::AUTO_TILE_DATABASE) = std::unique_ptr<IDatabase>(db); }
        inline void setSpriteDatabase(SpriteDatabase *db) { m_Databases.at(DATABASE::SPRITE_DATABASE) = std::unique_ptr<IDatabase>(db); }
        inline void setAnimationDatabase(AnimationDatabase *db) { m_Databases.at(DATABASE::ANIMATION_DATABASE) = std::unique_ptr<IDatabase>(db); }
        inline void setAnimationTypeDatabase(AnimationTypeDatabase *db) { m_Databases.at(DATABASE::ANIMATION_TYPE_DATABASE) = std::unique_ptr<IDatabase>(db); }
        inline void setWorldObjectDatabase(WorldObjectDatabase *db) { m_Databases.at(DATABASE::WORLD_OBJECT_DATABASE) = std::unique_ptr<IDatabase>(db); }
        inline void setDynamicObjectDatabase(DynamicObjectDatabase *db) { m_Databases.at(DATABASE::DYNAMIC_OBJECT_DATABASE) = std::unique_ptr<IDatabase>(db); }
        inline void setLocalisationDatabase(LocalisationDatabase *db) { m_Databases.at(DATABASE::LOCALISATION_DATABASE) = std::unique_ptr<IDatabase>(db); }
        inline void setMapDatabase(MAP_STRUCTURE::MapDatabase *db) { m_Databases.at(DATABASE::MAP_DATABASE) = std::unique_ptr<IDatabase>(db); }

        void clear();

        // in/output
        bool loadDatabase(const QString &projectPath, uint32 databases = ALL_DATABASES);
        bool saveDatabase(const QString &projectPath, uint32 databases = ALL_DATABASES);

    private:
        std::array<std::unique_ptr<IDatabase>, DATABASE_COUNT> m_Databases;
    };

    namespace TILE_SET
    {
        QPixmap createTileSetPixmap(const QString &path, const TileSetPrototype *pSet, const TileDatabase* pTileDB);
    }
}
#endif