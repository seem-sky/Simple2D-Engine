#ifndef UNIT_H
#define UNIT_H

#include "WorldObject.h"
#include "MovementGenerator.h"
#include "UnitAI.h"

class Unit : public WorldObject
{
public:
    Unit(uint32 uiGUID, Point<int> pos, DIRECTION dir = DIRECTION_DOWN, WALKMODE walkmode = WALKMODE_NONE, UnitAI *pAI = new UnitAI());
    virtual ~Unit(void);
    void Update(const ULONGLONG uiCurTime, const uint32 uiDiff);

    // Unit infos
    virtual void SetObjectInfo(const DATABASE::ObjectPrototype* pInfo);

    inline MovementGenerator* GetMovementGenerator() { return m_pMovement; }
    inline uint32 GetMovementSpeed() { return m_uiMovementSpeed; }
    inline void SetMovementSpeed(uint32 uiID) { SetMovementSpeed(WrapMovementSpeedID(uiID)); }
    inline void SetMovementSpeed(MOVEMENT_SPEED speed) { m_uiMovementSpeed = speed; }

    inline WALKMODE GetWalkmode() { return m_Walkmode; }

    void MovePosition(int XMove, int YMove, DIRECTION dir, uint32 time = 0);
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
    void SetTextureSource(const DATABASE::SpritePrototype *proto);
    virtual void DrawObject(LPD3DXSPRITE pSprite);
    inline void SetTextureSrcRct(uint32 uiSrcRct) { m_uiSpriteSector = uiSrcRct; }
    inline uint32 GetTextureSrcRct() { return m_uiSpriteSector; }

    // animation
    void UpdateAnimation(const ULONGLONG uiCurTime, const uint32 uiDiff);
    void FacingObject(WorldObject *pWho);
    void SetToStartSector();
    virtual void SetAnimationTime(ANIMATION_TIME time) { m_uiAnimationTime = time; }
    inline void SetDirection(DIRECTION dir)
    {
        if (!GetTextureSource())
            return;

        if (dir != DIRECTION_NONE)
        {
            m_uiDirection = (uint32)dir;
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
    uint32 m_uiSpriteSector;
    uint32 m_uiDirection;
    MovementGenerator *m_pMovement;

    // is player?
    bool m_bIsPlayer;

    // animation direction
    bool m_bAnimationDirection;
    bool m_bAllTimeAnimation;
    uint32 m_uiAnimation_Timer;
    ANIMATION_TIME m_uiAnimationTime;
    MOVEMENT_SPEED m_uiMovementSpeed;

    WALKMODE m_Walkmode;
};

#endif;