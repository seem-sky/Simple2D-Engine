#include "ObjectLayer.h"
#include "Map.h"
#include "Game.h"

ObjectLayer::ObjectLayer(void) : m_pOwnerMap(NULL), Layer()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "ObjectLayer : ";
    m_LayerType         = LAYER_TYPE_OBJECT_LAYER;
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
    for (std::multimap<int, WorldObject*>::const_iterator itr = m_v2VisibleObjects.begin(); itr != m_v2VisibleObjects.end(); ++itr)
        (*itr).second->DrawObject(pSprite);

    pDirect3D->EndSpriteDraw();
}

void ObjectLayer::ClearObjectList()
{
    for (WorldObjectList::const_iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
        delete *itr;
}

void ObjectLayer::AddWorldObject(WorldObject* pObject)
{
    if (!pObject)
        return;
    pObject->SetOwnerLayer(this);

    m_lObjects.push_back(pObject);
}

void ObjectLayer::UpdateLayer(const ULONGLONG uiCurTime, const UINT uiDiff)
{
    // clear visible objects
    m_v2VisibleObjects.clear();
    for (WorldObjectList::const_iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
    {
        if (!(*itr))
            continue;

        // check if object is visible, if true, store in visibleobject map
        if (IsObjectVisible(*itr))
            m_v2VisibleObjects.insert(std::make_pair<int, WorldObject*>((*itr)->GetBottomPosY(),*itr));

        (*itr)->Update(uiCurTime, uiDiff);
    }
}

bool ObjectLayer::IsObjectVisible(WorldObject *pObject)
{
    // if oject is not valid
    if (!pObject)
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
    int XPos = pObject->GetMapPosX();
    int YPos = pObject->GetMapPosY();
    UINT XSize = 0, YSize = 0;
    pObject->GetObjectSize(XSize, YSize);

    // get screen resolution
    UINT XScreenSize = 0, YScreenSize = 0;
    pGameInfo->GetWindowSize(XScreenSize, YScreenSize);

    // check X Pos
    if ((int)(XPos + XSize) < 0)
        return false;
    else if (XPos > (int)XScreenSize)
        return false;

    // check Y Pos
    if ((int)(YPos + YSize) < 0)
        return false;
    else if (YPos > (int)YScreenSize)
        return false;

    return true;
}