#ifndef UNIT_H
#define UNIT_H

#include "WorldObject.h"
#include "MovementGenerator.h"
#include "UnitAI.h"

class Unit : public WorldObject
{
public:
    Unit(UINT uiGUID, Point<int> pos, DIRECTION dir = DIRECTION_DOWN, WALKMODE walkmode = WALKMODE_NONE, UnitAI *pAI = new UnitAI());
    virtual ~Unit(void);
    void Update(const ULONGLONG uiCurTime, const UINT uiDiff);

    // Unit infos
    virtual void SetObjectInfo(const ObjectPrototype* pInfo);

    inline MovementGenerator* GetMovementGenerator() { return m_pMovement; }
    inline UINT GetMovementSpeed() { return m_uiMovementSpeed; }
    inline void SetMovementSpeed(UINT uiID) { SetMovementSpeed(WrapMovementSpeedID(uiID)); }
    inline void SetMovementSpeed(MOVEMENT_SPEED speed) { m_uiMovementSpeed = speed; }

    inline WALKMODE GetWalkmode() { return m_Walkmode; }

    void MovePosition(int XMove, int YMove, DIRECTION dir, UINT time = 0);
    inline bool IsMoving()
    {
        if (m_pMovement)
            return !m_pMovement->IsMoveCommandListEmpty();

        return false;
    }

    // if player has controle return true
    bool IsPlayer() { return m_bIsPlayer; }
    void SetPlayerControle(bool bControle = true) { m_bIsPlayer = bControle; }

    // sprite
    void SetTextureSource(const SpritePrototype *proto);
    virtual void DrawObject(LPD3DXSPRITE pSprite);
    inline void SetTextureSrcRct(UINT uiSrcRct) { m_uiSpriteSector = uiSrcRct; }
    inline UINT GetTextureSrcRct() { return m_uiSpriteSector; }

    // animation
    void UpdateAnimation(const ULONGLONG uiCurTime, const UINT uiDiff);
    void FacingObject(WorldObject *pWho);
    void SetToStartSector();
    virtual void SetAnimationTime(ANIMATION_TIME time) { m_uiAnimationTime = time; }
    inline void SetDirection(DIRECTION dir)
    {
        if (!GetTextureSource())
            return;

        if (dir != DIRECTION_NONE)
        {
            m_uiDirection = (UINT)dir;
            m_uiSpriteSector = m_uiDirection * GetTextureSource()->m_TextureInfo.Type.AnimatedObject.m_uiSpritesX + 1;
        }
    }
    inline DIRECTION GetDirection() { return (DIRECTION)m_uiDirection; }

    // AI
    virtual void SetAI(UnitAI* AI)
    {
        if (AI)
            WorldObject::SetAI(AI);
    }

    virtual UnitAI* GetAI()
    {
        if (UnitAI* AI = (UnitAI*)WorldObject::GetAI())
            return AI;

        return NULL;
    }

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
    MOVEMENT_SPEED m_uiMovementSpeed;

    WALKMODE m_Walkmode;
};

#endif;