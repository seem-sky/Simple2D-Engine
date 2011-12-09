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
};

class Unit : public WorldObject
{
public:
    Unit(void);
    virtual ~Unit(void);
    void Update(const UINT uiCurTime, const UINT uiDiff);
    virtual void DrawObject(LPD3DXSPRITE pSprite);
    void SetTextureSource(const SpritePrototype *proto);

    void MovePosition(int XMove, int YMove, UINT time = 0);

private:
    UINT m_uiSpriteSector;
    UINT m_uiDirection;
    MovementGenerator *m_pMovement;
};

#endif;