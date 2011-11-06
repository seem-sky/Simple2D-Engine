#include "Layer.h"

Layer::Layer(void)
{
    m_pSprite           = NULL;
    m_pDirect3D         = CDirect3D::Get();
    m_pLogfile          = CLogfile::Get();
    m_pRessourceManager = CRessourceManager::Get();
    m_LayerType         = LAYER_TYPE_LAYER;
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Layer : ";

    CreateSprite();
}

Layer::~Layer(void)
{
    if (m_pSprite)
    {
        m_pSprite->Release();
        m_pSprite = NULL;
    }

    if (m_pRessourceManager)
        m_pRessourceManager->Del();
}

void Layer::DrawLayer()
{
    BeginDraw();
    Draw();
    EndDraw();
}

HRESULT Layer::CreateSprite()
{
    if (m_pSprite)
    {
        m_pSprite->Release();
        m_pSprite = NULL;
    }

    LPDIRECT3DDEVICE9 pDevice = m_pDirect3D->GetDevice();
    HRESULT hr = D3DXCreateSprite(pDevice, &m_pSprite);
    return hr;
}

void Layer::BeginDraw(UINT DrawFlag)
{
    if (!m_pSprite)
        return;

    m_pSprite->Begin(DrawFlag);
}

void Layer::EndDraw()
{
    if (!m_pSprite)
        return;

    m_pSprite->End();
}