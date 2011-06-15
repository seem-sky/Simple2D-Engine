#include "WorldObject.h"

WorldObject::WorldObject(void)
{
    m_pTexture      = NULL;
    m_v3Position.x  = 0;
    m_v3Position.y  = 0;
    m_v3Position.z  = 0;

    m_UnitType      = UNIT_TYPE_WORLDOBJECT;
}

WorldObject::~WorldObject(void)
{
}

void WorldObject::SetTexture(std::string sTextureName)
{
    CRessourceManager *pRManager = CRessourceManager::Get();
    if (!pRManager)
        return;

    m_pTexture = pRManager->GetCharsetTexture(sTextureName);
}
void WorldObject::UpdateObject(const UINT uiCurTime, const UINT uiDiff)
{
    Update(uiCurTime, uiDiff);
}
