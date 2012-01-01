#include "MovementGenerator.h"
#include "Unit.h"
#include "Map.h"
#include "Game.h"

MovementGenerator::MovementGenerator(Unit *pObj) : m_pObj(pObj), m_v2CurMovement(0,0)
{
}

MovementGenerator::~MovementGenerator(void)
{
}

D3DXVECTOR2 MovementGenerator::Move2D(int x, int y, UINT uiMSECTime, bool Collision)
{
    sMoveCommand *pMove         = new sMoveCommand();
    pMove->m_bWithCollission    = Collision;
    pMove->m_MoveTime           = uiMSECTime;
    pMove->m_MoveX              = x / (float)uiMSECTime;
    pMove->m_MoveY              = y / (float)uiMSECTime;
    m_lMoveCommands.push_back(pMove);
    D3DXVECTOR2 v2Result((float)x, (float)y);
    return v2Result;
}

void MovementGenerator::UpdateMovement(const ULONGLONG uiCurTime, const UINT uiDiff)
{
    if (m_lMoveCommands.empty())
        return;

    if (!m_pObj)
        return;

    float MoveX = 0, MoveY = 0;
    D3DXVECTOR2 oldPos = (D3DXVECTOR2)m_pObj->GetPosition();
    if ((*m_lMoveCommands.begin())->m_MoveTime)
    {
        // update x
        if ((*m_lMoveCommands.begin())->m_MoveX)
        {
            if ((*m_lMoveCommands.begin())->m_MoveTime > uiDiff)
                m_v2CurMovement.x += (*m_lMoveCommands.begin())->m_MoveX * uiDiff;
            else
                m_v2CurMovement.x += (*m_lMoveCommands.begin())->m_MoveX * (*m_lMoveCommands.begin())->m_MoveTime;

            if (m_v2CurMovement.x > 0)
                MoveX = floor (m_v2CurMovement.x+0.00005f);
            else if (m_v2CurMovement.x < 0)
                MoveX = ceil (m_v2CurMovement.x-0.00005f);

            m_v2CurMovement.x -= MoveX;
        }

        // update y
        if ((*m_lMoveCommands.begin())->m_MoveY)
        {
            if ((*m_lMoveCommands.begin())->m_MoveTime > uiDiff)
                m_v2CurMovement.y += (*m_lMoveCommands.begin())->m_MoveY * uiDiff;
            else
                m_v2CurMovement.y += (*m_lMoveCommands.begin())->m_MoveY * (*m_lMoveCommands.begin())->m_MoveTime;

            if (m_v2CurMovement.y > 0)
                MoveY = floor (m_v2CurMovement.y+0.00005f);
            else if (m_v2CurMovement.y < 0)
                MoveY = ceil (m_v2CurMovement.y-0.00005f);

            m_v2CurMovement.y -= MoveY;
        }
    }

    if ((*m_lMoveCommands.begin())->m_bWithCollission)
    {
        D3DXVECTOR2 result(0,0);
        if (CanMove(oldPos, (D3DXVECTOR2)m_pObj->GetPosition() + D3DXVECTOR2 (MoveX, MoveY), result))
            m_pObj->ChangePosition((int)MoveX, (int)MoveY);
        else
            m_pObj->ChangePosition((int)result.x, (int)result.y);
    }
    else
        m_pObj->ChangePosition((int)MoveX, (int)MoveY);    

    if ((*m_lMoveCommands.begin())->m_MoveTime < uiDiff)
    {
        // if command finishes delete it
        m_v2CurMovement.x = 0;
        m_v2CurMovement.y = 0;
        RemoveMovementCommand((*m_lMoveCommands.begin()));
    }
    else
        (*m_lMoveCommands.begin())->m_MoveTime -= uiDiff;
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

bool MovementGenerator::CanMove(D3DXVECTOR2 oldPos, D3DXVECTOR2 newPos, D3DXVECTOR2 &result)
{
    result = D3DXVECTOR2(0,0);
    if (!m_pObj)
        return false;

    ObjectLayer *pLayer = m_pObj->GetOwnerLayer();
    if (!pLayer)
        return false;

    PassabilityFlag moveFlag = PASSABLE_NONE;
    if (oldPos.x < newPos.x)
        moveFlag = PASSABLE_RIGHT;
    else if (oldPos.x > newPos.x)
        moveFlag = PASSABLE_LEFT;

    if (oldPos.y < newPos.y)
        moveFlag = PASSABLE_DOWN;
    else if (oldPos.y > newPos.y)
        moveFlag = PASSABLE_UP;

    UINT XTileSize = 0, YTileSize = 0;
    if (CGame *pGame = CGame::Get())
        if (CGameInfo *pInfo = pGame->GetGameInfo())
            pInfo->GetMapTileSize(XTileSize, YTileSize);

    UINT XSize = 0, YSize = 0;
    m_pObj->GetObjectSize(XSize, YSize);
    RECT boundingRECT = {0,0,0,0};
    m_pObj->GetBoundingRect(boundingRECT);

    // check if terrain is passable
    if (Map *pMap = pLayer->GetOwnerMap())
    {
        int XMapPos = 0;
        int XMapPosEnd = 0;
        int YMapPos = 0;
        int YMapPosEnd = 0;
        if (moveFlag & PASSABLE_LEFT || moveFlag & PASSABLE_RIGHT)
        {
            // calc start pos
            if (moveFlag & PASSABLE_LEFT)
                XMapPos = (int)(oldPos.x - pMap->GetPositionX() + boundingRECT.left);
            else
                XMapPos = (int)(oldPos.x - pMap->GetPositionX() + boundingRECT.right-1);

            if (XMapPos < 0)
                XMapPos *= -1;

            // calc end pos
            if (moveFlag & PASSABLE_LEFT)
                XMapPosEnd = (int)(newPos.x - pMap->GetPositionX() + boundingRECT.left);
            else
                XMapPosEnd = (int)(newPos.x - pMap->GetPositionX() + boundingRECT.right-1);

            if (XMapPosEnd < 0)
                XMapPosEnd *= -1;

            for (int j = XMapPos / (int)XTileSize; moveFlag & PASSABLE_LEFT ? j >= XMapPosEnd / (int)XTileSize : j <= XMapPosEnd / (int)XTileSize;
                moveFlag & PASSABLE_LEFT ? j-- : j++)
            {
                YMapPos = (int)(oldPos.y - pMap->GetPositionY());
                if (YMapPos < 0)
                    YMapPos *= -1;
                for (UINT i = (YMapPos  + boundingRECT.top) / YTileSize; i <= (YMapPos + boundingRECT.bottom -1) / YTileSize; i++)
                {
                    if (!pMap->IsPassable((UINT)j , i, moveFlag))
                    {
                        result.x = (float)j * XTileSize - XMapPos;
                        if (moveFlag & PASSABLE_LEFT)
                            result.x += XTileSize;
                        else
                            result.x -= 1;

                        return false;
                    }
                }
            }
        }
        else if (moveFlag & PASSABLE_DOWN || moveFlag & PASSABLE_UP)
        {
            // calc start pos
            if (moveFlag & PASSABLE_UP)
                YMapPos = (int)(oldPos.y - pMap->GetPositionY() + boundingRECT.top);
            else
                YMapPos = (int)(oldPos.y - pMap->GetPositionY() + boundingRECT.bottom-1);

            if (YMapPos < 0)
                YMapPos *= -1;

            // calc end pos
            if (moveFlag & PASSABLE_UP)
                YMapPosEnd = (int)(newPos.y - pMap->GetPositionY() + boundingRECT.top);
            else
                YMapPosEnd = (int)(newPos.y - pMap->GetPositionY()  + boundingRECT.bottom-1);

            if (YMapPosEnd < 0)
                YMapPosEnd *= -1;

            for (int j = YMapPos / (int)YTileSize; moveFlag & PASSABLE_UP ? j >= YMapPosEnd / (int)YTileSize : j <= YMapPosEnd / (int)YTileSize;
                moveFlag & PASSABLE_UP ? j-- : j++)
            {
                XMapPos = (int)(oldPos.x - pMap->GetPositionX());
                if (XMapPos < 0)
                    XMapPos *= -1;
                for (UINT i = (XMapPos + boundingRECT.left) / XTileSize; i <= (XMapPos + boundingRECT.right-1) / XTileSize; i++)
                {
                    if (!pMap->IsPassable(i, (UINT)j, moveFlag))
                    {
                        result.y = (float)j * YTileSize - YMapPos;
                        if (moveFlag & PASSABLE_UP)
                            result.y += YTileSize;
                        else
                            result.y -= 1;

                        return false;
                    }
                }
            }
        }
    }

    return true;
}