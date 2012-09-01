#include "MovementGenerator.h"
#include "Map.h"
#include "Game.h"
#include "Collision.h"

MovementGenerator::MovementGenerator(Unit *pObj) : m_pObj(pObj), m_PathObj(pObj), m_bNewCommand(true), m_uiCurMoveTime(0)
{
}

MovementGenerator::~MovementGenerator(void)
{
}

void MovementGenerator::Move2D(int x, int y, uint32 uiMSECTime, DIRECTION dir, bool Collision)
{
    sMoveCommand *pMove         = new sMoveCommand();
    pMove->m_bWithCollission    = Collision;
    pMove->m_MoveTime           = uiMSECTime;
    pMove->m_MoveX              = x;
    pMove->m_MoveY              = y;
    pMove->m_Dir                = dir;
    m_lMoveCommands.push_back(pMove);
}

void MovementGenerator::Move2DRandom(uint32 uiRange, bool Collission)
{
    USHORT randomMove = rand() % 4;
    USHORT uiSteps    = rand() % 8 + 4;
    switch(randomMove)
    {
        // up
    case 0:
        Move2D(0, uiRange * uiSteps * (-1), 100 * uiSteps, (DIRECTION)randomMove);
        break;
        // right
    case 1:
        Move2D(uiRange * uiSteps, 0, 100 * uiSteps, (DIRECTION)randomMove);
        break;
        // down
    case 2:
        Move2D(0, uiRange * uiSteps, 100 * uiSteps, (DIRECTION)randomMove);
        break;
        // left
    case 3:
        Move2D(uiRange * uiSteps * (-1), 0, 100 * uiSteps, (DIRECTION)randomMove);
        break;
    }
}

bool MovementGenerator::UpdateMovement(const ULONGLONG uiCurTime, const uint32 uiDiff)
{
    if (!m_pObj)
        return false;

    if (m_lMoveCommands.empty())
    {
        if (!m_pObj->IsPlayer())
        {
            if (m_pObj->GetWalkmode() == WALKMODE_RANDOM)
                Move2DRandom(m_pObj->GetMovementSpeed());
            else
                return false;
        }
        else
            return false;
    }

    float MoveX = 0, MoveY = 0;
    // if a new movement command starts
    if (m_bNewCommand)
    {
        // set facing
        if (m_pObj->GetDirection() != (*m_lMoveCommands.begin())->m_Dir)
            m_pObj->SetDirection((*m_lMoveCommands.begin())->m_Dir);

        m_uiCurMoveTime = (*m_lMoveCommands.begin())->m_MoveTime;

        m_bNewCommand = false;
    }

    // update x
    if ((*m_lMoveCommands.begin())->m_MoveX)
    {
        if (m_uiCurMoveTime > uiDiff)
        {
            MoveX = (float)(*m_lMoveCommands.begin())->m_MoveX / (*m_lMoveCommands.begin())->m_MoveTime * uiDiff;
            if (MoveX > 0)
            {
                MoveX += m_fCurMovement.x - floor(m_fCurMovement.x);
                m_fCurMovement.x = floor(m_fCurMovement.x);
            }
            else if (MoveX < 0)
            {
                MoveX += m_fCurMovement.x - ceil(m_fCurMovement.x);
                m_fCurMovement.x = ceil(m_fCurMovement.x);
            }
        }
        else
        {
            if (m_fCurMovement.x >= 0)
                MoveX = (*m_lMoveCommands.begin())->m_MoveX - floor(m_fCurMovement.x);
            else if (m_fCurMovement.x < 0)
                MoveX = (*m_lMoveCommands.begin())->m_MoveX - ceil(m_fCurMovement.x);
        }

        m_fCurMovement.x += MoveX;
    }

    // update y
    if ((*m_lMoveCommands.begin())->m_MoveY)
    {
        if (m_uiCurMoveTime > uiDiff)
        {
            MoveY = (float)(*m_lMoveCommands.begin())->m_MoveY / (*m_lMoveCommands.begin())->m_MoveTime * uiDiff;
            if (MoveY > 0)
            {
                MoveY += m_fCurMovement.y - floor(m_fCurMovement.y);
                m_fCurMovement.y = floor(m_fCurMovement.y);
            }
            else if (MoveY < 0)
            {
                MoveY += m_fCurMovement.y - ceil(m_fCurMovement.y);
                m_fCurMovement.y = ceil(m_fCurMovement.y);
            }
        }
        else
        {
            if (m_fCurMovement.y >= 0)
                MoveY = (*m_lMoveCommands.begin())->m_MoveY - floor(m_fCurMovement.y);
            else if (m_fCurMovement.y < 0)
                MoveY = (*m_lMoveCommands.begin())->m_MoveY - ceil(m_fCurMovement.y);
        }

        m_fCurMovement.y += MoveY;
    }

    if ((*m_lMoveCommands.begin())->m_bWithCollission)
    {
        Point<int> result(0,0);
        Collission_Type CollissionWith = CheckMovement(m_pObj->GetPosition(), m_pObj->GetPosition() + Point<int>((int)MoveX, (int)MoveY), result);
        MoveX = (float)result.x;
        MoveY = (float)result.y;
        // collision while moved by pathfinder
        if (CollissionWith != COLLISSION_NONE && (*m_lMoveCommands.begin())->m_bPathfinderMove)
        {
            MovePointByPathfinder(m_PathObj.GetStoredEndPoint());
            return false;
        }
    }
    
    bool bMoving = false;
    if (MoveX != 0 || MoveY != 0)
    {
        m_pObj->ChangePosition((int)MoveX, (int)MoveY);
        if (m_pObj->IsPlayer())
        {
            // move map if its player
            if (Map* pMap = m_pObj->GetMap())
                pMap->ChangePosition(Point<int>((int)-MoveX, (int)-MoveY));

            CheckScriptPoint();
        }
        bMoving = true;
    }

    if (m_uiCurMoveTime < uiDiff || (MoveX == 0 && MoveY == 0))
    {
        // if command finishes delete it
        m_fCurMovement.x = 0;
        m_fCurMovement.y = 0;
        bool bPath = (*m_lMoveCommands.begin())->m_bPathfinderMove;
        RemoveMovementCommand((*m_lMoveCommands.begin()));
        m_bNewCommand = true;
        if (!IsMoveCommandListEmpty())
        {
            bMoving = true;
            // call AI reaction
            if (!(*m_lMoveCommands.begin())->m_bPathfinderMove)
                if (m_pObj->GetAI())
                    m_pObj->GetAI()->OnPathComplete();
        }
        else
        {
            // call AI reaction
            if (bPath)
                if (m_pObj->GetAI())
                    m_pObj->GetAI()->OnPathComplete();
        }
    }
    else
        m_uiCurMoveTime -= uiDiff;

    return bMoving;
}

void MovementGenerator::ClearMovement()
{
    for (MoveCommandList::iterator itr = m_lMoveCommands.begin(); itr != m_lMoveCommands.end(); ++itr)
        delete (*itr);
    m_lMoveCommands.clear();

    m_fCurMovement.x = 0;
    m_fCurMovement.y = 0;
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

Collission_Type MovementGenerator::CheckMovement(Point<int> oldPos, Point<int> newPos, Point<int> &result)
{
    Collission_Type CollissionWith = COLLISSION_NONE;
    if (Collision::CheckTileCollision(m_pObj, oldPos, newPos, result))
        CollissionWith = COLLISSION_TILE;

    if (result.x != 0 || result.y != 0)
    {
        newPos = oldPos + result;
        if (Collision::CheckObjectCollision(m_pObj, oldPos, newPos, result))
            CollissionWith = COLLISSION_OBJECT;
    }

    return CollissionWith;
}

void MovementGenerator::CheckScriptPoint()
{
    if (!m_pObj)
        return;

    Map *pMap = m_pObj->GetMap();
    if (!pMap)
        return;

    uint32 uiObjPosX = m_pObj->GetPositionX();
    uint32 uiObjPosY = m_pObj->GetPositionY();

    RECT boundingRect = { 0, 0, 0, 0 };
    m_pObj->GetBoundingRect(boundingRect);

    const ScriptPointLIST *pList = pMap->GetScriptPoints();
    Point<uint32> PointPos, PointSize;
    for (ScriptPointLIST::const_iterator itr = pList->begin(); itr != pList->end(); ++itr)
    {
        if (!(*itr))
            continue;

        PointPos = (*itr)->GetPosition();
        PointSize = (*itr)->GetSize();

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
            (*itr)->DoAction(m_pObj);
        }
    }
}

void MovementGenerator::MovePointByPathfinder(Point<uint32> uiPoint)
{
    ClearMovement();
    m_PathObj.GetValidPath(Point<uint32>((uint32)m_pObj->GetPositionX(), (uint32)m_pObj->GetPositionY()), uiPoint);
    // set all path commands as "MoveByPathfinder"
    for (MoveCommandList::iterator itr = m_lMoveCommands.begin(); itr != m_lMoveCommands.end(); ++itr)
    {
        if ((*itr))
            (*itr)->m_bPathfinderMove = true;
    }
    m_bNewCommand = true;
}