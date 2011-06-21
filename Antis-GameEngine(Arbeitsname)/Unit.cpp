#include "Unit.h"
#include "RessourceManager.h"

Unit::Unit(void)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Unit : ";
    m_UnitType          = UNIT_TYPE_UNIT;
    m_pMovement         = new MovementGenerator();
    m_pMovement->Init(static_cast<D3DXVECTOR2>(m_v3Position));
    m_uiTimer           = 1000;
}

Unit::~Unit(void)
{
    if (m_pMovement)
        delete m_pMovement;
}

void Unit::Update(const UINT uiCurTime, const UINT uiDiff)
{
    // WorldObject update
    WorldObject::Update(uiCurTime, uiDiff);

    // Update Movement
    if (!m_pMovement->IsMoveCommandListEmpty())
        m_v3Position += m_pMovement->UpdateMovement(uiCurTime, uiDiff);

    //if (m_uiTimer <= uiDiff)
    //{
    //    CRessourceManager *pRecManager = CRessourceManager::Get();
    //    if (m_pTexture->GetTextureName() == "asymptome.png")
    //    {
    //        pRecManager->ChangeTextureWith(m_pTexture, "barc.png");
    //        SetColor(0.5f, 1, 0, 1);
    //    }
    //    else
    //    {
    //        SetColor(1, 1, 1, 0.1f);
    //        pRecManager->ChangeTextureWith(m_pTexture, "asymptome.png");
    //    }
    //    
    //    m_uiTimer = 500;
    //}
    //else
    //    m_uiTimer -= uiDiff;
}
