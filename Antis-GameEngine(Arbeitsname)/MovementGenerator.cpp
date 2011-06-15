#include "MovementGenerator.h"

MovementGenerator::MovementGenerator(void)
{
    m_v2Position.x      = 0;
    m_v2Position.y      = 0;

    ClearMovement();

    m_bWithCollision    = false;
}

MovementGenerator::~MovementGenerator(void)
{
}

D3DXVECTOR2 MovementGenerator::Move2DWithoutCollision(int x, int y, UINT uiMSECTime)
{
    m_uiCurMoveTime = uiMSECTime;
    m_v2CurDiffMove.x = (float)x / uiMSECTime;
    m_v2CurDiffMove.y = (float)y / uiMSECTime;
    D3DXVECTOR2 v2Result((float)x, (float)y);
    m_bWithCollision = false;
    return v2Result;
}

//D3DXVECTOR2 MovementGenerator::Move2D(int x, int y, UINT uiMSECTime)
//{
//    m_v2CurMovement.x = (float)x;
//    m_v2CurMovement.y = (float)y;
//    m_uiCurMovementTime = uiMSECTime;
//    D3DXVECTOR2 v2Result((float)x, (float)y);
//    m_bWithCollision = true;
//    return v2Result;    
//}

D3DXVECTOR3 MovementGenerator::UpdateMovement(const UINT uiCurTime, const UINT uiDiff)
{
    float MoveX = 0, MoveY = 0;
    // update x
    if (m_uiCurMoveTime)
    {
        if (m_uiCurMoveTime > uiDiff)
            m_v2CurMovement.x += m_v2CurDiffMove.x * uiDiff;
        else
            m_v2CurMovement.x += m_v2CurDiffMove.x * m_uiCurMoveTime;

        MoveX = floor (m_v2CurMovement.x);
        m_v2CurMovement.x -= MoveX;
    }

    // update y
    if (m_uiCurMoveTime)
    {
        if (m_uiCurMoveTime > uiDiff)
            m_v2CurMovement.y += m_v2CurDiffMove.y * uiDiff;
        else
            m_v2CurMovement.y += m_v2CurDiffMove.y * m_uiCurMoveTime;

        MoveY = floor (m_v2CurMovement.y);
        m_v2CurMovement.y -= MoveY;
    }

    if (MoveX || MoveY)
    {
        if (m_uiCurMoveTime > uiDiff)
            m_uiCurMoveTime -= uiDiff;
        else
            ClearMovement();

        Move((int)MoveX, (int)MoveY);
    }

    return D3DXVECTOR3 (MoveX, MoveY, 0);
}

void MovementGenerator::Move(int x, int y)
{

}

void MovementGenerator::ClearMovement()
{
    m_v2CurMovement.x   = 0;
    m_v2CurMovement.y   = 0;
    m_uiCurMoveTime     = 0;
}

void MovementGenerator::Init(D3DXVECTOR2 v2Position)
{
    m_v2Position = v2Position;
}