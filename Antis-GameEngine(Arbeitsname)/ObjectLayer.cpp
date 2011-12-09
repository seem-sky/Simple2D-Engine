#include "ObjectLayer.h"
#include "Map.h"
#include "Game.h"

ObjectLayer::ObjectLayer(void) : Layer()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "ObjectLayer : ";
    m_LayerType         = LAYER_TYPE_OBJECT_LAYER;
    m_pOwnerMap         = NULL;
}

ObjectLayer::~ObjectLayer(void)
{
    ClearObjectList();
}

void ObjectLayer::Draw()
{
    CDirect3D *pDirect3D = CDirect3D::Get();
    if (!pDirect3D)
        return;

    LPD3DXSPRITE pSprite = pDirect3D->GetSpriteForDraw();
    if (!pSprite)
        return;

    // iterate through visible objects
    for (std::map<int, std::vector<WorldObject*>>::const_iterator itr = m_v2VisibleObjects.begin(); itr != m_v2VisibleObjects.end(); ++itr)
    {
        for (std::vector<WorldObject*>::const_iterator objItr = itr->second.begin(); objItr != itr->second.end(); ++objItr)
        {
            if (*objItr)
                (*objItr)->DrawObject(pSprite);
        }
    }
    pDirect3D->EndSpriteDraw();
}

void ObjectLayer::ClearObjectList()
{
    for (WorldObjectList::const_iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
        delete (*itr);
}

void ObjectLayer::AddWorldObject(WorldObject* pObject)
{
    if (!pObject)
        return;

    m_lObjects.push_back(pObject);
}

void ObjectLayer::UpdateLayer(const UINT uiCurTime, const UINT uiDiff)
{
    // clear visible objects
    m_v2VisibleObjects.clear();
    std::map<int, std::vector<WorldObject*>>::iterator objectItr;
    for (WorldObjectList::const_iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
    {
        if (!(*itr))
            continue;

        // check if object is visible, if true, store in visibleobject map
        if (IsObjectVisible(*itr))
        {
            objectItr = m_v2VisibleObjects.find((*itr)->GetPositionY());
            // if y row is not stored in visibleobjects, add it
            if (objectItr == m_v2VisibleObjects.end())
            {
                std::vector<WorldObject*> newVector;
                newVector.push_back(*itr);
                m_v2VisibleObjects.insert(std::make_pair<int, std::vector<WorldObject*>>((*itr)->GetPositionY(), newVector));
            }
            else
                objectItr->second.push_back(*itr);
        }

        (*itr)->Update(uiCurTime, uiDiff);
    }
}

bool ObjectLayer::IsObjectVisible(WorldObject *pObject)
{
    // if oject is not valid
    if (!pObject)
        return false;

    // if layer has no owner map
    if (!m_pOwnerMap)
        return false;

    // get game info (screen resolution)
    CGameInfo *pGameInfo = NULL;
    if (CGame *pGame = CGame::Get())
    {
        if (!(pGameInfo = pGame->GetGameInfo()))
            return false;
    }
    else
        return false;

    // get object resolution
    int XPos = pObject->GetPositionX();
    int YPos = pObject->GetPositionY();
    UINT XSize = 0, YSize = 0;
    pObject->GetObjectSize(XSize, YSize);

    // get map resolution
    int XMapPos = (int)m_pOwnerMap->GetPositionX();
    int YMapPos = (int)m_pOwnerMap->GetPositionY();

    // get screen resolution
    UINT XScreenSize = 0, YScreenSize = 0;
    pGameInfo->GetWindowSize(XScreenSize, YScreenSize);

    // check X Pos
    if (XMapPos + XPos + XSize < 0)
        return false;
    else if (XMapPos + XPos > (int)XScreenSize)
        return false;

    // check Y Pos
    if (YMapPos + YPos + YSize < 0)
        return false;
    else if (YMapPos + YPos > (int)YScreenSize)
        return false;

    return true;
}