#include "Database/Manager.h"
#include "Database/XML_Input.h"
#include "Database/XML_Output.h"

using namespace database;

TileDatabase& Manager::getTileDatabase()
{
    return m_TileDB;
}

const TileDatabase& Manager::getTileDatabase() const
{
    return m_TileDB;
}

TileSetDatabase& Manager::getTileSetDatabase()
{
    return m_TileSetDB;
}

const TileSetDatabase& Manager::getTileSetDatabase() const
{
    return m_TileSetDB;
}

AutoTileDatabase& Manager::getAutoTileDatabase()
{
    return m_AutoTileDB;
}

const AutoTileDatabase& Manager::getAutoTileDatabase() const
{
    return m_AutoTileDB;
}

SpriteDatabase& Manager::getSpriteDatabase()
{
    return m_SpriteDB;
}

const SpriteDatabase& Manager::getSpriteDatabase() const
{
    return m_SpriteDB;
}

AnimationAdditionPointDatabase& Manager::getAnimationAdditionPointDatabase()
{
    return m_AnimationAdditionPointDB;
}

const AnimationAdditionPointDatabase& Manager::getAnimationAdditionPointDatabase() const
{
    return m_AnimationAdditionPointDB;
}

AnimationAdditionRectDatabase& Manager::getAnimationAdditionRectDatabase()
{
    return m_AnimationAdditionRectDB;
}

const AnimationAdditionRectDatabase& Manager::getAnimationAdditionRectDatabase() const
{
    return m_AnimationAdditionRectDB;
}

AnimationDatabase& Manager::getAnimationDatabase()
{
    return m_AnimationDB;
}

const AnimationDatabase& Manager::getAnimationDatabase() const
{
    return m_AnimationDB;
}

AnimationAdditionTypeDatabase& Manager::getAnimationAdditionTypeDatabase()
{
    return m_AnimationAdditionTypeDB;
}

const AnimationAdditionTypeDatabase& Manager::getAnimationAdditionTypeDatabase() const
{
    return m_AnimationAdditionTypeDB;
}

WorldObjectDatabase& Manager::getWorldObjectDatabase()
{
    return m_WorldObjectDB;
}

const WorldObjectDatabase& Manager::getWorldObjectDatabase() const
{
    return m_WorldObjectDB;
}

MapDatabase& Manager::getMapDatabase()
{
    return m_MapDB;
}

const MapDatabase& Manager::getMapDatabase() const
{
    return m_MapDB;
}

// database store path
const char* DATABASE_FILE[] =
{
    "/Game/TileDatabase",
    "/Game/TileSetDatabase",
    "/Game/AutoTileDatabase",
    "/Game/SpriteDatabase",
    "/Game/AnimationDatabase",
    "/Game/AnimationAdditionPointDatabase",
    "/Game/AnimationAdditionRectDatabase",
    "/Game/AnimationAdditionTypeDatabase",
    "/Game/WorldObjectDatabase",
    "/Game/LocalsDatabase",
    "/Game/MapDatabase"
};

void Manager::load(const QString& projectDir)
{
    database::XML_Input input;
    input.read(m_TileDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::Tile)]);
    input.read(m_TileSetDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::TileSet)]);
    input.read(m_AutoTileDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::AutoTile)]);

    input.read(m_SpriteDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::Sprite)]);
    input.read(m_AnimationAdditionTypeDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::AnimationAdditionType)]);
    input.read(m_AnimationAdditionPointDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::AnimationAdditionPoint)]);
    input.read(m_AnimationAdditionRectDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::AnimationAdditionRect)]);

    input.read(m_WorldObjectDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::WorldObject)]);

    input.read(m_MapDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::Map)]);
}

void Manager::save(const QString& projectDir) const
{
    database::XML_Output output;
    output.write(m_TileDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::Tile)]);
    output.write(m_TileSetDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::TileSet)]);
    output.write(m_AutoTileDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::AutoTile)]);

    output.write(m_SpriteDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::Sprite)]);
    output.write(m_AnimationAdditionTypeDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::AnimationAdditionType)]);
    output.write(m_AnimationAdditionPointDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::AnimationAdditionPoint)]);
    output.write(m_AnimationAdditionRectDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::AnimationAdditionRect)]);

    output.write(m_MapDB, projectDir + DATABASE_FILE[static_cast<uint32>(DatabaseType::Map)]);
}
