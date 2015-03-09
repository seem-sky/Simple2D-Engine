#include "animation/AdditionData.h"
#include <stdexcept>

using namespace animation;

void AdditionData::setType(Type type)
{
    switch (type)
    {
    case animation::AdditionData::Type::point:
        m_Size.getHeight() = -1;
        m_Size.getWidth() = -1;
        break;
    }
}

AdditionData::Type AdditionData::getType() const
{
    return m_Type;
}

void AdditionData::setSize(const GEOMETRY::Size<int32>& size)
{
    m_Size = size;
    m_Type = Type::rect;
}

const GEOMETRY::Size<int32>& AdditionData::getSize() const
{
    if (m_Type != Type::rect)
        throw std::runtime_error("Is no rect type");
    return m_Size;
}

uint32 AdditionData::getAdditionTypeID() const
{
    return m_AdditionTypeID;
}

void AdditionData::setAdditionTypeID(uint32 ID)
{
    m_AdditionTypeID = ID;
}

uint32 AdditionData::minimumWidth()
{
    return 0;
}

uint32 AdditionData::maximumWidth()
{
    return 999999;
}

uint32 AdditionData::minimumHeight()
{
    return 0;
}

uint32 AdditionData::maximumHeight()
{
    return 999999;
}

bool animation::operator==(const AdditionData& lhs, const AdditionData& rhs)
{
    if (static_cast<const ItemData&>(lhs) != rhs || lhs.getType() != rhs.getType() || lhs.getAdditionTypeID() != rhs.getAdditionTypeID())
        return false;

    if (lhs.getType() == AdditionData::Type::rect && lhs.getSize() != rhs.getSize())
        return false;
    return true;
}

bool animation::operator!=(const AdditionData& lhs, const AdditionData& rhs)
{
    return !(lhs == rhs);
}
