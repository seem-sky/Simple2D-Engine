#ifndef ANIMATION_HOLDER_H
#define ANIMATION_HOLDER_H

#include "DatabasePrototypes.h"

namespace MAP
{
    namespace OBJECT
    {
        namespace ANIMATION
        {
            class AnimationHolder
            {
            private:
                void _changeCurrentFrame(uint32 uiFrame);

            public:
                AnimationHolder() : m_uiAnimationSpeed(100), m_uiTimer(0), m_uiCurrentFrame(0) {}

                void changeAnimation(DATABASE::ConstAnimationPrototypePtr pAnimation);
                inline DATABASE::ConstAnimationPrototypePtr getCurrentAnimation() const { return m_pAnimation; }
                inline uint32 getCurrentFrame() const { return m_uiCurrentFrame; }

                void update(uint32 uiDiff);

                inline uint32 getAnimationSpeed() const { return m_uiAnimationSpeed; }
                inline void setAnimationSpeed(uint32 uiAnimationSpeed) { m_uiAnimationSpeed = uiAnimationSpeed; _changeCurrentFrame(m_uiCurrentFrame); }

            private:
                DATABASE::ConstAnimationPrototypePtr m_pAnimation;
                uint32 m_uiCurrentFrame;
                uint32 m_uiTimer;
                uint16 m_uiAnimationSpeed;
            };
        }
    }
}
#endif