#ifndef UNIT_H
#define UNIT_H

#include "WorldObject.h"
#include "MovementGenerator.h"

enum DIRECTION
{
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_NONE,
};

enum ANIMATION_TIME
{
    ANIMATION_TIME_VERY_SLOW    = 1000,
    ANIMATION_TIME_SLOW         = 500,
    ANIMATION_TIME_NORMAL       = 250,
    ANIMATION_TIME_FAST         = 125,
    ANIMATION_TIME_VERY_FAST    = 65,
};

class Unit : public WorldObject
{
public:
    Unit(void);
    virtual ~Unit(void);
    void Update(const UINT uiCurTime, const UINT uiDiff);

    void MovePosition(int XMove, int YMove, UINT time = 0);
    inline bool IsMoving()
    {
        if (m_pMovement)
            return !m_pMovement->IsMoveCommandListEmpty();

        return false;
    }

    // if player has controle return true
    bool IsPlayer() { return m_bIsPlayer; }

    // sprite
    void SetTextureSource(const SpritePrototype *proto);
    virtual void DrawObject(LPD3DXSPRITE pSprite);
    inline void SetTextureSrcRct(UINT uiSrcRct) { m_uiSpriteSector = uiSrcRct; }
    inline UINT GetTextureSrcRct() { return m_uiSpriteSector; }
    void UpdateAnimation(const UINT uiCurTime, const UINT uiDiff);
    virtual void SetAnimationTime(ANIMATION_TIME time) { m_uiAnimationTime = time; }

    inline void SetDirection(DIRECTION dir)
    {
        if (dir != DIRECTION_NONE)
        {
            m_uiDirection = (UINT)dir;
            m_uiSpriteSector = m_uiDirection * GetTextureSource()->m_TextureInfo.Type.AnimatedObject.m_uiSpritesX + 1;
        }
    }
    inline DIRECTION GetDirection() { return (DIRECTION)m_uiDirection; }

private:
    UINT m_uiSpriteSector;
    UINT m_uiDirection;
    MovementGenerator *m_pMovement;

    // is player?
    bool m_bIsPlayer;

    // animation direction
    bool m_bAnimationDirection;
    bool m_bAllTimeAnimation;
    UINT m_uiAnimation_Timer;
    ANIMATION_TIME m_uiAnimationTime;
};

#endif;