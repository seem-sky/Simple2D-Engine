#include "animation/SpriteData.h"
#include <math_extensions.h>

using namespace animation;

SpriteData::SpriteData(uint32 ID)
    : ItemData(), m_SpriteID(ID)
{}

bool SpriteData::isEmpty() const
{
    return m_SpriteID == 0;
}

uint32 SpriteData::getID() const
{
    return m_SpriteID;
}

bool animation::operator==(const SpriteData& lhs, const SpriteData& rhs)
{
    return lhs.getID() == rhs.getID() &&
        static_cast<const ItemData&>(lhs) == rhs;
}

bool animation::operator!=(const SpriteData& lhs, const SpriteData& rhs)
{
    return !(lhs == rhs);
}
