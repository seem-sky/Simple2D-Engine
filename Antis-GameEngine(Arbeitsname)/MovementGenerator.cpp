#include "MovementGenerator.h"

MovementGenerator::MovementGenerator(D3DXVECTOR2 *pPosition) : m_pPosition(pPosition), m_v2CurMovement(0,0)
{
}

MovementGenerator::~MovementGenerator(void)
{
}

D3DXVECTOR2 MovementGenerator::Move2DWithoutCollision(int x, int y, UINT uiMSECTime)
{
    sMoveCommand *pMove         = new sMoveCommand();
    pMove->m_bWithCollission    = false;
    pMove->m_MoveTime           = uiMSECTime;
    pMove->m_MoveX              = x / (float)uiMSECTime;
    pMove->m_MoveY              = y / (float)uiMSECTime;
    m_lMoveCommands.push_back(pMove);
    D3DXVECTOR2 v2Result((float)x, (float)y);
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

void MovementGenerator::UpdateMovement(const ULONGLONG uiCurTime, const UINT uiDiff)
{
    if (m_lMoveCommands.empty())
        return;

    float MoveX = 0, MoveY = 0;
    // update x
    if ((*m_lMoveCommands.begin())->m_MoveTime)
    {
        if ((*m_lMoveCommands.begin())->m_MoveTime > uiDiff)
            m_v2CurMovement.x += (*m_lMoveCommands.begin())->m_MoveX * uiDiff;
        else
            m_v2CurMovement.x += (*m_lMoveCommands.begin())->m_MoveX * (*m_lMoveCommands.begin())->m_MoveTime;

        MoveX = floor (m_v2CurMovement.x);
        m_v2CurMovement.x -= MoveX;
    }

    // update y
    if ((*m_lMoveCommands.begin())->m_MoveTime)
    {
        if ((*m_lMoveCommands.begin())->m_MoveTime > uiDiff)
            m_v2CurMovement.y += (*m_lMoveCommands.begin())->m_MoveY * uiDiff;
        else
            m_v2CurMovement.y += (*m_lMoveCommands.begin())->m_MoveY * (*m_lMoveCommands.begin())->m_MoveTime;

        MoveY = floor (m_v2CurMovement.y);
        m_v2CurMovement.y -= MoveY;
    }

    if ((*m_lMoveCommands.begin())->m_MoveTime < uiDiff)
    {
        // if command finishes delete it
        m_v2CurMovement.x = 0;
        m_v2CurMovement.y = 0;
        RemoveMovementCommand((*m_lMoveCommands.begin()));
    }
    else
    {
        (*m_lMoveCommands.begin())->m_MoveTime -= uiDiff;
        // update object position
        *m_pPosition += D3DXVECTOR2 (MoveX, MoveY);
    }
}

void MovementGenerator::Move(int x, int y)
{

}

void MovementGenerator::ClearMovement()
{
    for (MoveCommandList::iterator itr = m_lMoveCommands.begin(); itr != m_lMoveCommands.end(); ++itr)
        delete (*itr);
    m_lMoveCommands.clear();

    m_v2CurMovement.x = 0;
    m_v2CurMovement.y = 0;
}

void MovementGenerator::RemoveMovementCommand(sMoveCommand* pCommand)
{
    for (MoveCommandList::iterator itr = m_lMoveCommands.begin(); itr != m_lMoveCommands.end(); ++itr)
    {
        if ((*itr) == pCommand)
        {
            m_lMoveCommands.remove(pCommand);
            delete pCommand;
            return;
        }
    }
}