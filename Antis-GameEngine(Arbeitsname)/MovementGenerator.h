#ifndef MOVEMENT_GENERATOR_H
#define MOVEMENT_GENERATOR_H

#include <d3dx9.h>

class MovementGenerator
{
public:
    MovementGenerator(void);
    ~MovementGenerator(void);

    D3DXVECTOR2 Move2DWithoutCollision(int x, int y, UINT uiMSECTime);
    /*D3DXVECTOR2 Move2D(int x, int y, UINT uiMSECTime);*/
    D3DXVECTOR3 UpdateMovement(const UINT uiCurTime, const UINT uiDiff);
    void ClearMovement();
    void Init(D3DXVECTOR2 v2Position);

protected:
    D3DXVECTOR2 m_v2Position;

private:
    void Move(int x, int y);

    D3DXVECTOR2 m_v2CurMovement;
    D3DXVECTOR2 m_v2CurDiffMove;
    UINT m_uiCurMoveTime;
    bool m_bWithCollision;
};

#endif;