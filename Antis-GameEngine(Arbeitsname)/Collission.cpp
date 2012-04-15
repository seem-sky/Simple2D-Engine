#include "Collission.h"
#include "Game.h"

bool Collission::CheckTileCollission(Unit* pWho, Point<int> oldPos, Point<int> newPos, Point<int> &result)
{
    if (!pWho)
        return true;

    ObjectLayer *pLayer = pWho->GetOwnerLayer();
    if (!pLayer)
        return true;

    result = newPos - oldPos;

    PassabilityFlag moveFlag = PASSABLE_NONE;
    if (oldPos.x < newPos.x)
        moveFlag = PASSABLE_RIGHT;
    else if (oldPos.x > newPos.x)
        moveFlag = PASSABLE_LEFT;

    if (oldPos.y < newPos.y)
        moveFlag = PASSABLE_DOWN;
    else if (oldPos.y > newPos.y)
        moveFlag = PASSABLE_UP;

    Point<UINT> tileSize;
    if (CGame *pGame = CGame::Get())
        if (GameInfo *pInfo = pGame->GetGameInfo())
            tileSize = pInfo->GetMapTileSize();

    UINT XSize = 0, YSize = 0;
    pWho->GetObjectSize(XSize, YSize);
    RECT boundingRECT = {0,0,0,0};
    pWho->GetBoundingRect(boundingRECT);

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
                    result.x -= XMapPosEnd;
                // right border
                else if (XMapPosEnd > (int)(pInfo->m_uiX * tileSize.x))
                    result.x -= XMapPosEnd - pInfo->m_uiX * tileSize.x;
            }

            for (int j = XMapPos / (int)tileSize.x; moveFlag & PASSABLE_LEFT ? j >= XMapPosEnd / (int)tileSize.x : j <= XMapPosEnd / (int)tileSize.x;
                moveFlag & PASSABLE_LEFT ? j-- : j++)
            {
                YMapPos = (int)(oldPos.y);
                if (YMapPos < 0)
                    YMapPos *= -1;

                bool bBreak = false;
                for (UINT i = (YMapPos  + boundingRECT.top) / tileSize.y; i <= (YMapPos + boundingRECT.bottom -1) / tileSize.y; i++)
                {
                    if (!pMap->IsPassable((UINT)j , i, moveFlag))
                    {
                        bBreak = true;
                        result.x = j * tileSize.x - XMapPos;
                        if (moveFlag & PASSABLE_LEFT)
                            result.x += tileSize.x;
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
                    result.y -= YMapPosEnd;
                // right border
                else if (YMapPosEnd > (int)(pInfo->m_uiY * tileSize.y))
                    result.y -= YMapPosEnd - pInfo->m_uiY * tileSize.y;
            }

            for (int j = YMapPos / (int)tileSize.y; moveFlag & PASSABLE_UP ? j >= YMapPosEnd / (int)tileSize.y : j <= YMapPosEnd / (int)tileSize.y;
                moveFlag & PASSABLE_UP ? j-- : j++)
            {
                XMapPos = (int)(oldPos.x);
                if (XMapPos < 0)
                    XMapPos *= -1;

                bool bBreak = false;
                for (UINT i = (XMapPos + boundingRECT.left) / tileSize.x; i <= (XMapPos + boundingRECT.right-1) / tileSize.x; i++)
                {
                    if (!pMap->IsPassable(i, (UINT)j, moveFlag))
                    {
                        bBreak = true;
                        result.y = j * tileSize.y - YMapPos;
                        if (moveFlag & PASSABLE_UP)
                            result.y += tileSize.y;
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

    // calculate result
    if (moveFlag & PASSABLE_LEFT)
    {
        if (result.x < newPos.x - oldPos.x || result.x > 0)
            result.x = 0;
    }
    else if (moveFlag & PASSABLE_RIGHT)
    {
        if (result.x > newPos.x - oldPos.x || result.x < 0)
            result.x = 0;
    }

    if (moveFlag & PASSABLE_UP)
    {
        if (result.y < newPos.y - oldPos.y || result.y > 0)
            result.y = 0;
    }
    else if (moveFlag & PASSABLE_DOWN)
    {
        if (result.y > newPos.y - oldPos.y || result.y < 0)
            result.y = 0;
    }

    if (newPos.x - oldPos.x != result.x)
        return true;
    else if (newPos.y - oldPos.y != result.y)
        return true;

    return false;
}

bool Collission::CheckObjectCollission(Unit* pWho, Point<int> oldPos, Point<int> newPos, Point<int> &result)
{
    if (!pWho)
        return true;

    ObjectLayer *pLayer = pWho->GetOwnerLayer();
    if (!pLayer)
        return true;

    result = newPos - oldPos;

    // check move direction
    PassabilityFlag moveFlag = PASSABLE_NONE;
    if (oldPos.x < newPos.x)
        moveFlag = PASSABLE_RIGHT;
    else if (oldPos.x > newPos.x)
        moveFlag = PASSABLE_LEFT;

    if (oldPos.y < newPos.y)
        moveFlag = PASSABLE_DOWN;
    else if (oldPos.y > newPos.y)
        moveFlag = PASSABLE_UP;

    // get bounding rect size
    UINT XSize = 0, YSize = 0;
    pWho->GetObjectSize(XSize, YSize);
    RECT boundingRECT = {0,0,0,0};
    pWho->GetBoundingRect(boundingRECT);

    // check collission with objects
    const WorldObjectList *objectList = pLayer->GetObjectsOnLayer();
    for (WorldObjectList::const_iterator itr = objectList->begin(); itr != objectList->end(); ++itr)
    {
        if (*itr == pWho)
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
                    continue;
                }
            }
        }
    }
    
    // calculate result
    if (moveFlag & PASSABLE_LEFT)
    {
        if (result.x < newPos.x - oldPos.x || result.x > 0)
            result.x = 0;
    }
    else if (moveFlag & PASSABLE_RIGHT)
    {
        if (result.x > newPos.x - oldPos.x || result.x < 0)
            result.x = 0;
    }

    if (moveFlag & PASSABLE_UP)
    {
        if (result.y < newPos.y - oldPos.y || result.y > 0)
            result.y = 0;
    }
    else if (moveFlag & PASSABLE_DOWN)
    {
        if (result.y > newPos.y - oldPos.y || result.y < 0)
            result.y = 0;
    }

    if (newPos.x - oldPos.x != result.x)
        return true;
    else if (newPos.y - oldPos.y != result.y)
        return true;

    return false;
}