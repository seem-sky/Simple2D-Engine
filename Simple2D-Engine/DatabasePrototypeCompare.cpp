#include "Database/Prototype/Compare.h"

using namespace database;

bool prototype::compare(const Base& lhs, const Base& rhs)
{
    return lhs.getID() == rhs.getID() &&
        lhs.getName() == rhs.getName();
}

bool prototype::compare(const Resource& lhs, const Resource& rhs)
{
    return compare(static_cast<const Base&>(lhs), rhs) &&
        lhs.getPathName() == rhs.getPathName();
}

bool prototype::compare(const Texture& lhs, const Texture& rhs)
{
    return compare(static_cast<const Resource&>(lhs), rhs) &&
        lhs.getTransparencyColor() == rhs.getTransparencyColor();
}

bool prototype::compare(const Tile& lhs, const Tile& rhs)
{
    return compare(static_cast<const Texture&>(lhs), rhs) &&
        lhs.getPassability() == rhs.getPassability();
}

bool prototype::compare(const TileSet& lhs, const TileSet& rhs)
{
    return compare(static_cast<const Base&>(lhs), rhs) &&
        lhs.m_Tiles == rhs.m_Tiles;
}

bool prototype::compare(const AutoTile& lhs, const AutoTile& rhs)
{
    return compare(static_cast<const Base&>(lhs), rhs) &&
        lhs.m_Tiles == rhs.m_Tiles;
}

bool prototype::compare(const Sprite& lhs, const Sprite& rhs)
{
    return compare(static_cast<const Texture&>(lhs), rhs);
}

bool prototype::compare(const AnimationAdditionType& lhs, const AnimationAdditionType& rhs)
{
    return compare(static_cast<const Base&>(lhs), rhs);
}

bool prototype::compare(const AnimationAddition& lhs, const AnimationAddition& rhs)
{
    return compare(static_cast<const Base&>(lhs), rhs) &&
        lhs.getAdditionType() == rhs.getAdditionType() &&
        lhs.m_Sprites == rhs.m_Sprites;
}

bool prototype::compare(const AnimationAdditionPoint& lhs, const AnimationAdditionPoint& rhs)
{
    return compare(static_cast<const AnimationAddition&>(lhs), rhs) &&
        lhs.getPosition() == rhs.getPosition();
}

bool prototype::compare(const AnimationAdditionRect& lhs, const AnimationAdditionRect& rhs)
{
    return compare(static_cast<const AnimationAddition&>(lhs), rhs);
}

bool prototype::compare(const Animation& lhs, const Animation& rhs)
{
    return compare(static_cast<const Base&>(lhs), rhs) &&
        lhs.getFrames() == rhs.getFrames();
}
