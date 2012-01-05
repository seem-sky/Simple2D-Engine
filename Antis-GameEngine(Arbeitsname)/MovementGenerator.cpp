#include "MovementGenerator.h"
#include "Map.h"
#include "Game.h"

MovementGenerator::MovementGenerator(Unit *pObj) : m_pObj(pObj), m_v2CurMovement(0,0)
{
}

MovementGenerator::~MovementGenerator(void)
{
}

void MovementGenerator::Move2D(int x, int y, UINT uiMSECTime, bool Collision)
{
    sMoveCommand *pMove         = new sMoveCommand();
    pMove->m_bWithCollission    = Collision;
    pMove->m_MoveTime           = uiMSECTime;
    pMove->m_MoveX              = x / (float)uiMSECTime;
    pMove->m_MoveY              = y / (float)uiMSECTime;
    m_lMoveCommands.push_back(pMove);
}

void MovementGenerator::Move2DRandom(UINT uiRange, bool Collission)
{
    USHORT randomMove = rand() % 4;
    USHORT uiSteps    = rand() % 8 + 4;
    switch(randomMove)
    {
        // up
    case 0:
        Move2D(0, uiRange * uiSteps * (-1), 100 * uiSteps);
        m_pObj->SetDirection((DIRECTION)randomMove);
        break;
        // right
    case 1:
        Move2D(uiRange * uiSteps, 0, 100 * uiSteps);
        m_pObj->SetDirection((DIRECTION)randomMove);
        break;
        // down
    case 2:
        Move2D(0, uiRange * uiSteps, 100 * uiSteps);
        m_pObj->SetDirection((DIRECTION)randomMove);
        break;
        // left
    case 3:
        Move2D(uiRange * uiSteps * (-1), 0, 100 * uiSteps);
        m_pObj->SetDirection((DIRECTION)randomMove);
        break;
    }
}

void MovementGenerator::UpdateMovement(const ULONGLONG uiCurTime, const UINT uiDiff)
{
    if (!m_pObj)
        return;

    if (m_lMoveCommands.empty())
    {
        if (!m_pObj->IsPlayer())
        {
            if (m_pObj->GetWalkmode() == WALKMODE_RANDOM)
                Move2DRandom(m_pObj->GetMovementSpeed());
            else
                return;
        }
        else
            return;
    }

    int MoveX = 0, MoveY = 0;
    Point<int> oldPos = m_pObj->GetPosition();
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
                MoveX = (int)floor(m_v2CurMovement.x+0.00005f);
            else if (m_v2CurMovement.x < 0)
                MoveX = (int)ceil(m_v2CurMovement.x-0.00005f);

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
                MoveY = (int)floor(m_v2CurMovement.y+0.00005f);
            else if (m_v2CurMovement.y < 0)
                MoveY = (int)ceil(m_v2CurMovement.y-0.00005f);

            m_v2CurMovement.y -= MoveY;
        }
    }

    if ((*m_lMoveCommands.begin())->m_bWithCollission)
    {
        Point<int> result(0,0);
        CheckMovement(oldPos, m_pObj->GetPosition() + Point<int>(MoveX, MoveY), result);
        MoveX = (int)result.x;
        MoveY = (int)result.y;
    }

    m_pObj->ChangePosition((int)MoveX, (int)MoveY);
    if (m_pObj->IsPlayer())
    {
        // move map if its player
        if (Map* pMap = m_pObj->GetMap())
            pMap->ChangePosition(Point<int>(-MoveX, -MoveY));

        CheckScriptPoint();
    }

    // stop animation if not moving
    if (MoveX == 0 && MoveY == 0)
        m_pObj->SetToStartSector();

    if ((*m_lMoveCommands.begin())->m_MoveTime < uiDiff || (MoveX == 0 && MoveY == 0))
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

bool MovementGenerator::CheckMovement(Point<int> oldPos, Point<int> newPos, Point<int> &result)
{
    result = newPos - oldPos;
    bool bCollission = false;
    if (!m_pObj)
        return true;

    ObjectLayer *pLayer = m_pObj->GetOwnerLayer();
    if (!pLayer)
        return true;

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
                XMapPos = (int)(oldPos.x + boundingRECT.left);
            else
                XMapPos = (int)(oldPos.x + boundingRECT.right-1);

            // calc end pos
            if (moveFlag & PASSABLE_LEFT)
                XMapPosEnd = (int)(newPos.x + boundingRECT.left);
            else
                XMapPosEnd = (int)(newPos.x + boundingRECT.right-1);

            // check map border
            if (const MapInfo *pInfo = pMap->GetMapInfo())
            {
                // left border
                if (XMapPosEnd < 0)
                {
                    result.x -= XMapPosEnd;
                    bCollission = true;
                }
                // right border
                else if (XMapPosEnd > (int)(pInfo->m_uiX * XTileSize))
                {
                    result.x -= XMapPosEnd - pInfo->m_uiX * XTileSize;
                    bCollission = true;
                }
            }

            for (int j = XMapPos / (int)XTileSize; moveFlag & PASSABLE_LEFT ? j >= XMapPosEnd / (int)XTileSize : j <= XMapPosEnd / (int)XTileSize;
                moveFlag & PASSABLE_LEFT ? j-- : j++)
            {
                YMapPos = (int)(oldPos.y);
                if (YMapPos < 0)
                    YMapPos *= -1;

                bool bBreak = false;
                for (UINT i = (YMapPos  + boundingRECT.top) / YTileSize; i <= (YMapPos + boundingRECT.bottom -1) / YTileSize; i++)
                {
                    if (!pMap->IsPassable((UINT)j , i, moveFlag))
                    {
                        bCollission = true;
                        bBreak = true;
                        result.x = j * XTileSize - XMapPos;
                        if (moveFlag & PASSABLE_LEFT)
                            result.x += XTileSize;
                        else
                            result.x -= 1;

                        break;
                    }
                }
                // break while collission
                if (bBreak)
                    break;
            }
        }
        if (moveFlag & PASSABLE_DOWN || moveFlag & PASSABLE_UP)
        {
            // calc start pos
            if (moveFlag & PASSABLE_UP)
                YMapPos = (int)(oldPos.y + boundingRECT.top);
            else
                YMapPos = (int)(oldPos.y + boundingRECT.bottom-1);

            // calc end pos
            if (moveFlag & PASSABLE_UP)
                YMapPosEnd = (int)(newPos.y + boundingRECT.top);
            else
                YMapPosEnd = (int)(newPos.y  + boundingRECT.bottom-1);

            // check map border
            if (const MapInfo *pInfo = pMap->GetMapInfo())
            {
                // left border
                if (YMapPosEnd < 0)
                {
                    result.y -= YMapPosEnd;
                    bCollission = true;
                }
                // right border
                else if (YMapPosEnd > (int)(pInfo->m_uiY * YTileSize))
                {
                    result.y -= YMapPosEnd - pInfo->m_uiY * YTileSize;
                    bCollission = true;
                }
            }

            for (int j = YMapPos / (int)YTileSize; moveFlag & PASSABLE_UP ? j >= YMapPosEnd / (int)YTileSize : j <= YMapPosEnd / (int)YTileSize;
                moveFlag & PASSABLE_UP ? j-- : j++)
            {
                XMapPos = (int)(oldPos.x);
                if (XMapPos < 0)
                    XMapPos *= -1;

                bool bBreak = false;
                for (UINT i = (XMapPos + boundingRECT.left) / XTileSize; i <= (XMapPos + boundingRECT.right-1) / XTileSize; i++)
                {
                    if (!pMap->IsPassable(i, (UINT)j, moveFlag))
                    {
                        bBreak = true;
                        bCollission = true;
                        result.y = j * YTileSize - YMapPos;
                        if (moveFlag & PASSABLE_UP)
                            result.y += YTileSize;
                        else
                            result.y -= 1;

                        break;
                    }
                }
                // break while collission
                if (bBreak)
                    break;
            }
        }
    }

    // check collission with objects
    const WorldObjectList *objectList = pLayer->GetObjectsOnLayer();
    for (WorldObjectList::const_iterator itr = objectList->begin(); itr != objectList->end(); ++itr)
    {
        if (*itr == m_pObj)
            continue;

        newPos = result + oldPos;
        Point<int> objPos = (*itr)->GetPosition();
        RECT objBound = {0,0,0,0};
        (*itr)->GetBoundingRect(objBound);
        if (moveFlag & PASSABLE_LEFT)
        {
            if (objPos.x + objBound.right >= newPos.x + boundingRECT.left
                && objPos.x  + objBound.right <= oldPos.x + boundingRECT.left)
            {
                if ((objPos.y + objBound.bottom > newPos.y + boundingRECT.top
                    && objPos.y  + objBound.bottom < newPos.y + boundingRECT.bottom) ||
                    (objPos.y + objBound.top > newPos.y + boundingRECT.top
                    && objPos.y  + objBound.top < newPos.y + boundingRECT.bottom) ||
                    (objPos.y + objBound.top <= newPos.y + boundingRECT.top
                    && objPos.y  + objBound.bottom >= newPos.y + boundingRECT.bottom))
                {
                    result.x = (objPos.x + objBound.right) - (oldPos.x + boundingRECT.left);
                    bCollission = true;
                    continue;
                }
            }
        }
        else if (moveFlag & PASSABLE_RIGHT)
        {
            if (objPos.x + objBound.left <= newPos.x + boundingRECT.right
                && objPos.x  + objBound.left >= oldPos.x + boundingRECT.right)
            {
                if ((objPos.y + objBound.bottom > newPos.y + boundingRECT.top
                    && objPos.y  + objBound.bottom < newPos.y + boundingRECT.bottom) ||
                    (objPos.y + objBound.top > newPos.y + boundingRECT.top
                    && objPos.y  + objBound.top < newPos.y + boundingRECT.bottom) ||
                    (objPos.y + objBound.top <= newPos.y + boundingRECT.top
                    && objPos.y  + objBound.bottom >= newPos.y + boundingRECT.bottom))
                {
                    result.x = (objPos.x + objBound.left) - (oldPos.x + boundingRECT.right);
                    bCollission = true;
                    continue;
                }
            }
        }
        
        if (moveFlag & PASSABLE_UP)
        {
            if (objPos.y + objBound.bottom >= newPos.y + boundingRECT.top
                && objPos.y  + objBound.bottom <= oldPos.y + boundingRECT.top)
            {
                if ((objPos.x + objBound.right > newPos.x + boundingRECT.left
                    && objPos.x  + objBound.right < newPos.x + boundingRECT.right) ||
                    (objPos.x + objBound.left > newPos.x + boundingRECT.left
                    && objPos.x  + objBound.left < newPos.x + boundingRECT.right) ||
                    (objPos.x + objBound.left <= newPos.x + boundingRECT.left
                    && objPos.x  + objBound.right >= newPos.x + boundingRECT.right))
                {
                    result.y = (objPos.y + objBound.bottom) - (oldPos.y + boundingRECT.top);
                    bCollission = true;
                    continue;
                }
            }
        }
        else if (moveFlag & PASSABLE_DOWN)
        {
            if (objPos.y + objBound.top <= newPos.y + boundingRECT.bottom
                && objPos.y  + objBound.top >= oldPos.y + boundingRECT.bottom)
            {
                if ((objPos.x + objBound.right > newPos.x + boundingRECT.left
                    && objPos.x  + objBound.right < newPos.x + boundingRECT.right) ||
                    (objPos.x + objBound.left > newPos.x + boundingRECT.left
                    && objPos.x  + objBound.left < newPos.x + boundingRECT.right) ||
                    (objPos.x + objBound.left <= newPos.x + boundingRECT.left
                    && objPos.x  + objBound.right >= newPos.x + boundingRECT.right))
                {
                    result.y = (objPos.y + objBound.top) - (oldPos.y + boundingRECT.bottom);
                    bCollission = true;
                    continue;
                }
            }
        }
    }
    return bCollission;
}

void MovementGenerator::CheckScriptPoint()
{
    if (!m_pObj)
        return;

    Map *pMap = m_pObj->GetMap();
    if (!pMap)
        return;

    UINT uiObjPosX = m_pObj->GetPositionX();
    UINT uiObjPosY = m_pObj->GetPositionY();

    RECT boundingRect = { 0, 0, 0, 0 };
    m_pObj->GetBoundingRect(boundingRect);

    const ScriptPointLIST *pList = pMap->GetScriptPoints();
    Point<UINT> PointPos, PointSize;
    for (ScriptPointLIST::const_iterator itr = pList->begin(); itr != pList->end(); ++itr)
    {
        if (!(*itr))
            continue;

        PointPos = (*itr)->GetPosition();
        PointSize = (*itr)->GetSize();

        bool bSuccess = false;
            // left side of m_pObj
        if ((PointPos.x <= uiObjPosX + boundingRect.left && PointPos.x + PointSize.x >= uiObjPosX + boundingRect.left) ||
            // right side of m_pObj
            (PointPos.x <= uiObjPosX + boundingRect.right && PointPos.x + PointSize.x >= uiObjPosX + boundingRect.right))
        {
                // top left edge of Obj is in point
            if ((PointPos.y <= uiObjPosY + boundingRect.top && PointPos.y + PointSize.y >= uiObjPosY + boundingRect.top) ||
                // bottom left edge of Obj is in point
                (PointPos.y <= uiObjPosY + boundingRect.bottom && PointPos.y + PointSize.y >= uiObjPosY + boundingRect.bottom) ||
                // left side without edges is in point
                (PointPos.y >= uiObjPosY + boundingRect.top && PointPos.y + PointSize.y <= uiObjPosY + boundingRect.bottom))
            bSuccess = true;
        }


        if (bSuccess)
        {
            (*itr)->DoAction(m_pObj);
        }
    }
}