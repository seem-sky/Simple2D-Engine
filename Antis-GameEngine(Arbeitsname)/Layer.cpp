#include "Layer.h"

Layer::Layer(void)
{
    m_pSprite           = NULL;
    m_pDirect3D         = CDirect3D::Get();
    m_pLogfile          = CLogfile::Get();
    m_pRessourceManager = CRessourceManager::Get();
    m_LayerType         = LAYER_TYPE_LAYER;
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Layer : ";

    D3DXCreateSprite(m_pDirect3D->GetDevice(), &m_pSprite);
}

Layer::~Layer(void)
{
    if (m_pSprite)
        m_pSprite->Release();
    if (m_pRessourceManager)
        m_pRessourceManager->Del();
}

void Layer::DrawLayer()
{
    BeginDraw();
    Draw();
    EndDraw();
}
