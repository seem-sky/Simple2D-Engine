#pragma once

#include "Base.h"
#include "Prototype/Derived.h"

namespace database
{
    typedef Base<prototype::Tile, 65000> TileDatabase;
    typedef Base<prototype::AutoTile, 250> AutoTileDatabase;
    typedef Base<prototype::TileSet, 65000> TileSetDatabase;

    typedef Base<prototype::Sprite, 65000> SpriteDatabase;
    typedef Base<prototype::AnimationAdditionType, 65000> AnimationAdditionTypeDatabase;
    typedef Base<prototype::AnimationAdditionPoint, 65000> AnimationAdditionPointDatabase;
    typedef Base<prototype::AnimationAdditionRect, 65000> AnimationAdditionRectDatabase;
    typedef Base<prototype::Animation, 65000> AnimationDatabase;

    typedef Base<prototype::WorldObject, 65000> WorldObjectDatabase;
    typedef Base<prototype::Map, 65000> MapDatabase;
}
