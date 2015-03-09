#pragma once

#include "Derived.h"

namespace database
{
    class Manager
    {
    public:
        enum class DatabaseType
        {
            Tile,
            TileSet,
            AutoTile,

            Sprite,
            Animation,
            AnimationAdditionPoint,
            AnimationAdditionRect,
            AnimationAdditionType,

            WorldObject,

            Localisation,

            Map
        };

        TileDatabase& getTileDatabase();
        const TileDatabase& getTileDatabase() const;

        TileSetDatabase& getTileSetDatabase();
        const TileSetDatabase& getTileSetDatabase() const;

        AutoTileDatabase& getAutoTileDatabase();
        const AutoTileDatabase& getAutoTileDatabase() const;

        SpriteDatabase& getSpriteDatabase();
        const SpriteDatabase& getSpriteDatabase() const;

        AnimationAdditionPointDatabase& getAnimationAdditionPointDatabase();
        const AnimationAdditionPointDatabase& getAnimationAdditionPointDatabase() const;

        AnimationAdditionRectDatabase& getAnimationAdditionRectDatabase();
        const AnimationAdditionRectDatabase& getAnimationAdditionRectDatabase() const;

        AnimationDatabase& getAnimationDatabase();
        const AnimationDatabase& getAnimationDatabase() const;

        AnimationAdditionTypeDatabase& getAnimationAdditionTypeDatabase();
        const AnimationAdditionTypeDatabase& getAnimationAdditionTypeDatabase() const;

        WorldObjectDatabase& getWorldObjectDatabase();
        const WorldObjectDatabase& getWorldObjectDatabase() const;

        MapDatabase& getMapDatabase();
        const MapDatabase& getMapDatabase() const;

        void load(const QString& projectDir);
        void save(const QString& projectDir) const;

    private:
        TileDatabase m_TileDB;
        TileSetDatabase m_TileSetDB;
        AutoTileDatabase m_AutoTileDB;

        SpriteDatabase m_SpriteDB;

        AnimationDatabase m_AnimationDB;
        AnimationAdditionPointDatabase m_AnimationAdditionPointDB;
        AnimationAdditionRectDatabase m_AnimationAdditionRectDB;
        AnimationAdditionTypeDatabase m_AnimationAdditionTypeDB;

        WorldObjectDatabase m_WorldObjectDB;
        MapDatabase m_MapDB;
    };
}
