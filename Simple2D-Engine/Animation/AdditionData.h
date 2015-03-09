#pragma once

#include "ItemData.h"
#include <geometry/Size.h>
#include <vector>

namespace animation
{
    class AdditionData : public ItemData
    {
    public:
        AdditionData() = default;

        enum class Type
        {
            point,
            rect
        };
        void setType(Type type);
        Type getType() const;

        void setSize(const GEOMETRY::Size<int32>& size);
        const GEOMETRY::Size<int32>& getSize() const;

        uint32 getAdditionTypeID() const;
        void setAdditionTypeID(uint32 ID);

        static uint32 minimumWidth();
        static uint32 maximumWidth();
        static uint32 minimumHeight();
        static uint32 maximumHeight();

    private:
        uint32 m_AdditionTypeID = 0;
        GEOMETRY::Size<int32> m_Size;
        Type m_Type = Type::point;

    };

    bool operator==(const AdditionData& lhs, const AdditionData& rhs);
    bool operator!=(const AdditionData& lhs, const AdditionData& rhs);
    
    typedef std::vector<AdditionData> AdditionVector;
}
