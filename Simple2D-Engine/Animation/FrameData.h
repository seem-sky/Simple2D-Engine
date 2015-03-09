#pragma once

#include "SpriteData.h"
#include "AdditionData.h"

namespace animation
{
    class FrameData
    {
        friend bool operator==(const FrameData& lhs, const FrameData& rhs);
    public:
        const SpriteData& getSprite(uint32 index) const;
        void insertSprite(uint32 index, const SpriteData& sprite);
        void setSprite(uint32 index, const SpriteData& sprite);
        void removeSprite(uint32 index);
        void addSprite(const SpriteData& sprite);
        uint32 getSpriteCount() const;
        void setSpriteCount(uint32 size);

        const AdditionData& getAddition(uint32 index) const;
        void insertAddition(uint32 index, const AdditionData& data);
        void setAddition(uint32 index, const AdditionData& data);
        void removeAddition(uint32 index);
        void addAddition(const AdditionData& data);
        uint32 getAdditionCount() const;
        void setAdditionCount(uint32 size);

        const GEOMETRY::Point<int32>& getOffset() const;

        uint32 getTime() const;
        void setTime(uint32 time);

        bool isEmpty() const;

    private:
        SpriteVector m_Sprites;
        AdditionVector m_Additions;
        GEOMETRY::Point<int32> m_Offset;
        uint32 m_Time = 0;
    };

    bool operator==(const FrameData& lhs, const FrameData& rhs);
    bool operator!=(const FrameData& lhs, const FrameData& rhs);

    typedef std::vector<FrameData> FrameVector;
}
