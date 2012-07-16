#include "Pathfinder.h"
#include "Map.h"
#include "Unit.h"
#include "Game.h"
#include "Collision.h"

Pathfinder::Pathfinder(Unit *pOwner) : m_pOwner(pOwner)
{
}

Pathfinder::~Pathfinder(void)
{
    ResetPath();
}

bool Pathfinder::GetValidPath(Point<UINT> uiStartPos, Point<UINT> uiEndPos)
{
    ResetPath();
    m_uiStoredEndPoint = uiEndPos;
    Map *pMap = NULL;
    if (!m_pOwner)
        return false;

    pMap = m_pOwner->GetMap();
    if (!pMap)
        return false;

    Point<UINT> uiStartTile = pMap->CalcPixToTile(uiStartPos);
    Point<UINT> uiEndTile   = pMap->CalcPixToTile(uiEndPos);

    // add starting point to closed nodes.
    Point<UINT> range = GetRangeBetween(uiStartPos, uiEndPos);
    PathNode *CurNode = new PathNode(uiStartPos, PATH_START, 1, range.x + range.y, NULL);
    m_ClosedNodes.insert(std::make_pair<Point<UINT>, PathNode*>(CurNode->m_uiPos, CurNode));

    bool bFinishPath = false;
    Point<int> move;
    while(!bFinishPath)
    {
        // check tiles around tile
        for (USHORT i = 0; i < 4; i++)
        {
            PathNode *NewNode = NULL;
            switch(i)
            {
                // top
            case 0:
                if (!Collision::CheckTileCollision(m_pOwner, Point<int>(CurNode->m_uiPos.x, CurNode->m_uiPos.y), Point<int>(CurNode->m_uiPos.x, CurNode->m_uiPos.y-1), move))
                {
                    range = GetRangeBetween(Point<UINT>(CurNode->m_uiPos.x, CurNode->m_uiPos.y -1), uiEndPos);
                    NewNode = new PathNode(Point<UINT>(CurNode->m_uiPos.x, CurNode->m_uiPos.y -1), PATH_UP, CurNode->m_uiGPrice + 1, range.x + range.y, CurNode);
                }
                else
                    continue;
                break;
                // left
            case 1:
                if (!Collision::CheckTileCollision(m_pOwner, Point<int>(CurNode->m_uiPos.x, CurNode->m_uiPos.y), Point<int>(CurNode->m_uiPos.x-1, CurNode->m_uiPos.y), move))
                {
                    range = GetRangeBetween(Point<UINT>(CurNode->m_uiPos.x -1, CurNode->m_uiPos.y), uiEndPos);
                    NewNode = new PathNode(Point<UINT>(CurNode->m_uiPos.x -1, CurNode->m_uiPos.y), PATH_LEFT, CurNode->m_uiGPrice + 1, range.x + range.y, CurNode);
                }
                else
                    continue;
                break;
                // bottom
            case 2:
                if (!Collision::CheckTileCollision(m_pOwner, Point<int>(CurNode->m_uiPos.x, CurNode->m_uiPos.y), Point<int>(CurNode->m_uiPos.x, CurNode->m_uiPos.y+1), move))
                {
                    range = GetRangeBetween(Point<UINT>(CurNode->m_uiPos.x, CurNode->m_uiPos.y +1), uiEndPos);
                    NewNode = new PathNode(Point<UINT>(CurNode->m_uiPos.x, CurNode->m_uiPos.y +1), PATH_DOWN, CurNode->m_uiGPrice + 1, range.x + range.y, CurNode);
                }
                else
                    continue;
                break;
                // right
            case 3:
                if (!Collision::CheckTileCollision(m_pOwner, Point<int>(CurNode->m_uiPos.x, CurNode->m_uiPos.y), Point<int>(CurNode->m_uiPos.x+1, CurNode->m_uiPos.y), move))
                {
                    range = GetRangeBetween(Point<UINT>(CurNode->m_uiPos.x +1, CurNode->m_uiPos.y), uiEndPos);
                    NewNode = new PathNode(Point<UINT>(CurNode->m_uiPos.x +1, CurNode->m_uiPos.y), PATH_RIGHT, CurNode->m_uiGPrice + 1,  range.x + range.y, CurNode);
                }
                else
                    continue;
                break;
            default:
                continue;
            }

            if (IsStoredInClosedList(NewNode))
            {
                delete NewNode;
                continue;
            }
            else if (IsStoredInOpenList(NewNode))
            {
                if (PathNode *tempNode = GetStoredPathNode(NewNode->m_uiPos))
                {
                    if (NewNode->m_uiGPrice < tempNode->m_uiGPrice)
                    {
                        *tempNode = *NewNode;
                        delete NewNode;
                    }
                }
            }
            else
                m_OpenNodes.insert(std::make_pair<Point<UINT>, PathNode*>(NewNode->m_uiPos, NewNode));
        }

        // get PathNode with lowest F
        NodeList::iterator storedItr = m_OpenNodes.end();
        UINT uiLowestF = 0-1;
        if (!m_OpenNodes.empty())
        {
            for (NodeList::iterator itr = m_OpenNodes.begin(); itr != m_OpenNodes.end(); ++itr)
            {
                if (!itr->second)
                    continue;

                // if F lower than stored F, store curNode
                if (itr->second->m_uiGPrice + itr->second->m_uiHCalc < uiLowestF)
                {
                    uiLowestF = itr->second->m_uiGPrice + itr->second->m_uiHCalc;
                    storedItr = itr;
                }
            }
        }
        // if open nodes is empty get node with lowest f from closed list or return false
        else
        {
            //if (bFindNearestPoint)
            //{
                storedItr = m_ClosedNodes.end();
                for (NodeList::iterator itr = m_ClosedNodes.begin(); itr != m_ClosedNodes.end(); ++itr)
                {
                    if (!itr->second)
                        continue;

                    // if F lower than stored F, store curNode
                    if (itr->second->m_uiGPrice + itr->second->m_uiHCalc < uiLowestF)
                    {
                        uiLowestF = itr->second->m_uiGPrice + itr->second->m_uiHCalc;
                        storedItr = itr;
                    }
                }
            //    // if find a point, break loop an read out path
            //    if (storedItr != m_ClosedNodes.end())
            //    {
            //        CurNode = storedItr->second;
            //        break;
            //    }
            //}

            return false;
        }

        // switch node from openlist to closed list
        if (storedItr != m_OpenNodes.end())
        {
            CurNode = storedItr->second;
            m_OpenNodes.erase(storedItr);
            m_ClosedNodes.insert(std::make_pair<Point<UINT>, PathNode*>(CurNode->m_uiPos, CurNode));
        }

        if (CurNode->m_uiPos == uiEndPos)
            bFinishPath = true;
    }

    // read out of path
    std::list<PathDir> path;
    path.push_back(PATH_END);
    while(CurNode)
    {
        path.push_front(CurNode->m_PathDir);
        CurNode = CurNode->m_pLastNode;
    }

    // add path to owner movelist
    PathDir lastDir = PATH_START;
    USHORT lastDirCount = 1;
    Point<UINT> uiTileSize;
    if (CGame *pGame = CGame::Get())
        if (GameInfo *pInfo = pGame->GetGameInfo())
            uiTileSize = pInfo->GetMapTileSize();

    Point<int> uiStart;
    uiStart.x = (int)uiStartPos.x;
    uiStart.y = (int)uiStartPos.y;
    RECT boundingRECT = {0,0,0,0};
    m_pOwner->GetBoundingRect(boundingRECT);
    Point<int> uiMove;
    for (std::list<PathDir>::iterator itr = path.begin(); itr != path.end(); ++itr)
    {
        // start node, check if unit is on one tile or two, if two, move to one
        if (*itr == PATH_START)
        {
            std::list<PathDir>::iterator nextItr = itr;
            ++nextItr;
            continue;
        }

        if (*itr != lastDir || itr == --path.end())
        {
            switch(lastDir)
            {
            case PATH_UP:
                if (uiMove.y != 0)
                    m_pOwner->MovePosition(0, uiMove.y, DIRECTION_UP, (uiMove.y*(-1))/m_pOwner->GetMovementSpeed()*100);
                break;
            case PATH_DOWN:
                if (uiMove.y != 0)
                    m_pOwner->MovePosition(0, uiMove.y, DIRECTION_DOWN, uiMove.y/m_pOwner->GetMovementSpeed()*100);
                break;
            case PATH_LEFT:
                if (uiMove.x != 0)
                    m_pOwner->MovePosition(uiMove.x, 0, DIRECTION_LEFT, (uiMove.x*(-1))/m_pOwner->GetMovementSpeed()*100);
                break;
            case PATH_RIGHT:
                if (uiMove.x != 0)
                    m_pOwner->MovePosition(uiMove.x, 0, DIRECTION_RIGHT, uiMove.x/m_pOwner->GetMovementSpeed()*100);
                break;
            default:
                break;
            }
            lastDir = *itr;
            uiStart += uiMove;
            uiMove.x = 0;
            uiMove.y = 0;
        }

        switch(*itr)
        {
        case PATH_UP:
            uiMove.y -= 1;
            break;
        case PATH_DOWN:
            uiMove.y += 1;
            break;
        case PATH_LEFT:
            uiMove.x -= 1;
            break;
        case PATH_RIGHT:
            uiMove.x += 1;
            break;
        default:
            break;
        }
    }

    return true;
}

void Pathfinder::ResetPath()
{
    // delete open nodes
    for (NodeList::iterator itr = m_OpenNodes.begin(); itr != m_OpenNodes.end(); ++itr)
    {
        if (itr->second)
            delete itr->second;
    }
    m_OpenNodes.clear();

    // delete closed nodes
    for (NodeList::iterator itr = m_ClosedNodes.begin(); itr != m_ClosedNodes.end(); ++itr)
    {
        if (itr->second)
            delete itr->second;
    }
    m_ClosedNodes.clear();
}

bool Pathfinder::IsStoredInClosedList(PathNode *Node)
{
    NodeList::iterator itr = m_ClosedNodes.find(Node->m_uiPos);
    if (itr != m_ClosedNodes.end())
        return true;

    return false;
}

bool Pathfinder::IsStoredInOpenList(PathNode *Node)
{
    NodeList::iterator itr = m_OpenNodes.find(Node->m_uiPos);
    if (itr != m_OpenNodes.end())
        return true;

    return false;
}

PathNode* Pathfinder::GetStoredPathNode(Point<UINT> uiTile)
{
    NodeList::iterator itr = m_OpenNodes.find(uiTile);
    if (itr != m_OpenNodes.end())
        return itr->second;

    itr = m_ClosedNodes.find(uiTile);
    if (itr != m_ClosedNodes.end())
        return itr->second;

    return NULL;
}

Point<UINT> Pathfinder::GetRangeBetween(Point<UINT> start, Point<UINT> end)
{
    Point<UINT> range;
    if (start.x > end.x)
        range.x = start.x - end.x;
    else
        range.x = end.x - start.x;

    if (start.y > end.y)
        range.y = start.y - end.y;
    else
        range.y = end.y - start.y;

    return range;
}