#include "animation/FrameData.h"

using namespace animation;

const SpriteData& FrameData::getSprite(uint32 index) const
{
    return m_Sprites.at(index);
}

void FrameData::setSprite(uint32 index, const SpriteData& sprite)
{
    m_Sprites.at(index) = sprite;
}

void FrameData::removeSprite(uint32 index)
{
    m_Sprites.erase(m_Sprites.begin() + index);
}

void FrameData::insertSprite(uint32 index, const SpriteData& sprite)
{
    m_Sprites.insert(m_Sprites.begin() + index, sprite);
}

void FrameData::addSprite(const SpriteData& sprite)
{
    m_Sprites.push_back(sprite);
}

uint32 FrameData::getSpriteCount() const
{
    return static_cast<uint32>(m_Sprites.size());
}

void FrameData::setSpriteCount(uint32 size)
{
    m_Sprites.resize(size);
}

const GEOMETRY::Point<int32>& FrameData::getOffset() const
{
    return m_Offset;
}

const AdditionData& FrameData::getAddition(uint32 index) const
{
    return m_Additions.at(index);
}

void FrameData::setAddition(uint32 index, const AdditionData& data)
{
    m_Additions.at(index) = data;
}
void FrameData::removeAddition(uint32 index)
{
    m_Additions.erase(m_Additions.begin() + index);
}

void FrameData::insertAddition(uint32 index, const AdditionData& data)
{
    m_Additions.insert(m_Additions.begin() + index, data);
}

void FrameData::addAddition(const AdditionData& data)
{
    m_Additions.push_back(data);
}

uint32 FrameData::getAdditionCount() const
{
    return static_cast<uint32>(m_Additions.size());
}

void FrameData::setAdditionCount(uint32 size)
{
    m_Additions.resize(size);
}

uint32 FrameData::getTime() const
{
    return m_Time;
}

void FrameData::setTime(uint32 time)
{
    m_Time = time;
}

bool FrameData::isEmpty() const
{
    return m_Time == 0;
}

bool animation::operator==(const FrameData& lhs, const FrameData& rhs)
{
    return lhs.m_Additions == rhs.m_Additions &&
        lhs.m_Sprites == rhs.m_Sprites &&
        lhs.m_Time == rhs.m_Time &&
        lhs.m_Offset == rhs.m_Offset;
}

bool animation::operator!=(const FrameData& lhs, const FrameData& rhs)
{
    return !(lhs == rhs);
}

