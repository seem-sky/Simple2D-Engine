#include "animation/ItemData.h"
#include <math_extensions.h>

using namespace animation;

GEOMETRY::Point<int32> ItemData::getPosition() const
{
    return m_Position;
}

void ItemData::setPosition(const GEOMETRY::Point<int32>& pos)
{
    m_Position = pos;
}

int32 ItemData::getX() const
{
    return m_Position.getX();
}

int32 ItemData::getY() const
{
    return m_Position.getY();
}

void ItemData::setX(int32 x)
{
    m_Position.getX() = x;
}

void ItemData::setY(int32 y)
{
    m_Position.getY() = y;
}

float ItemData::getZValue() const
{
    return m_ZValue;
}

void ItemData::setZValue(float z)
{
    m_ZValue = MATH::between(z, minimumZValue(), maximumZValue());
}

float ItemData::getScale() const
{
    return m_Scale;
}

void ItemData::setScale(float scale)
{
    m_Scale = MATH::between(scale, minimumScale(), maximumScale());
}

float ItemData::getRotation() const
{
    return m_Rotation;
}

void ItemData::setRotation(float rotation)
{
    m_Rotation = MATH::between(rotation, minimumRotation(), maximumRotation());
}

float ItemData::getOpacity() const
{
    return m_Opacity;
}

void ItemData::setOpacity(float opacity)
{
    m_Opacity = MATH::between(opacity, minimumOpacity(), maximumOpacity());
}

float ItemData::minimumZValue()
{
    return -1000.0f;
}

float ItemData::maximumZValue()
{
    return 1000.0f;
}

float ItemData::minimumScale()
{
    return 0.0f;
}

float ItemData::maximumScale()
{
    return 1000.0f;
}

float ItemData::minimumRotation()
{
    return 0.0f;
}

float ItemData::maximumRotation()
{
    return 359.99f;
}

float ItemData::minimumOpacity()
{
    return 0.0f;
}

float ItemData::maximumOpacity()
{
    return 1.0f;
}

bool animation::operator==(const ItemData& lhs, const ItemData& rhs)
{
    return lhs.getPosition() == rhs.getPosition() &&
        MATH::aboutTheSame(lhs.getOpacity(), rhs.getOpacity()) &&
        MATH::aboutTheSame(lhs.getRotation(), rhs.getRotation()) &&
        MATH::aboutTheSame(lhs.getScale(), rhs.getScale()) &&
        MATH::aboutTheSame(lhs.getZValue(), rhs.getZValue());
}

bool animation::operator!=(const ItemData& lhs, const ItemData& rhs)
{
    return !(lhs == rhs);
}
