#pragma once

#include "ItemData.h"
#include <vector>

namespace animation
{
    class SpriteData : public ItemData
    {
    public:
        SpriteData(uint32 ID = 0);

        bool isEmpty() const;

        uint32 getID() const;

    private:
        uint32 m_SpriteID = 0;
    };

    bool operator==(const SpriteData& lhs, const SpriteData& rhs);
    bool operator!=(const SpriteData& lhs, const SpriteData& rhs);

    typedef std::vector<SpriteData> SpriteVector;
}
