#pragma once

#include <Global.h>
#include <geometry/Point.h>

namespace animation
{
    class ItemData
    {
    public:
        ItemData() = default;

        GEOMETRY::Point<int32> getPosition() const;
        void setPosition(const GEOMETRY::Point<int32>& pos);
        int32 getX() const;
        void setX(int32 x);
        int32 getY() const;
        void setY(int32 y);

        float getZValue() const;
        void setZValue(float z);
        float getScale() const;
        void setScale(float scale);
        float getRotation() const;
        void setRotation(float rotation);
        float getOpacity() const;
        void setOpacity(float opacity);

        static float minimumZValue();
        static float maximumZValue();
        static float minimumScale();
        static float maximumScale();
        static float minimumRotation();
        static float maximumRotation();
        static float minimumOpacity();
        static float maximumOpacity();

    private:
        uint32 m_SpriteID = 0;
        GEOMETRY::Point<int32> m_Position;
        float m_ZValue = 0.0f;
        float m_Scale = 1.0f;
        float m_Rotation = 0.0f;
        float m_Opacity = 1.0f;
    };

    bool operator==(const ItemData& lhs, const ItemData& rhs);
    bool operator!=(const ItemData& lhs, const ItemData& rhs);
}
