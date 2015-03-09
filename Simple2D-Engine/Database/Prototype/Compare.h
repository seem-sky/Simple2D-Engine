#pragma once

#include "Derived.h"

namespace database
{
    namespace prototype
    {
        bool compare(const Base& lhs, const Base& rhs);
        bool compare(const Resource& lhs, const Resource& rhs);
        bool compare(const Texture& lhs, const Texture& rhs);

        bool compare(const Tile& lhs, const Tile& rhs);
        bool compare(const TileSet& lhs, const TileSet& rhs);
        bool compare(const AutoTile& lhs, const AutoTile& rhs);

        bool compare(const Sprite& lhs, const Sprite& rhs);

        bool compare(const AnimationAdditionType& lhs, const AnimationAdditionType& rhs);
        bool compare(const AnimationAddition& lhs, const AnimationAddition& rhs);
        bool compare(const AnimationAdditionPoint& lhs, const AnimationAdditionPoint& rhs);
        bool compare(const AnimationAdditionRect& lhs, const AnimationAdditionRect& rhs);
        bool compare(const Animation& lhs, const Animation& rhs);
    }
}
