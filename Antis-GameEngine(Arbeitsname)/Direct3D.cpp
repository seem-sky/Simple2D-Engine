#include "Direct3D.h"
#include "ResourceManager.h"


CDirect3D::CDirect3D() : m_lpD3D(0), m_lpD3DDevice(0), m_lpBackBuffer(0), m_uiSpriteIndex(0), TSingleton()
{
    m_sLogLocationName              = LOGFILE_ENGINE_LOG_NAME + "Direct3D : ";
}

CDirect3D::~CDirect3D()
{
    CleanUp();
}

// Direct3D initialisieren
bool CDirect3D::Initialize(HWND hWnd, const UINT uiScreenWidth, const UINT uiScreenHeight, const bool bWindowed)
{
    // create Direct3D object
    if(FAILED(m_lpD3D = Direct3DCreate9(D3D_SDK_VERSION)))
    {
        // can´t create Direct3D Object
        BASIC_LOG(m_sLogLocationName + "Unable to create Direct3D Object.");
        return false;
    }

    ZeroMemory(&m_PParams, sizeof(m_PParams));

    m_PParams.SwapEffect       = D3DSWAPEFFECT_DISCARD;
    m_PParams.hDeviceWindow    = hWnd;
    m_PParams.Windowed         = bWindowed;

    m_PParams.BackBufferWidth  = uiScreenWidth;
    m_PParams.BackBufferHeight = uiScreenHeight;

    m_PParams.BackBufferFormat = D3DFMT_X8R8G8B8;

    m_PParams.Flags            = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    // create direct3D device
    if(FAILED(m_lpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_MIXED_VERTEXPROCESSING+D3DCREATE_MULTITHREADED, &m_PParams, &m_lpD3DDevice)))
    {
        // can´t create direct3D device
        ERROR_LOG(m_sLogLocationName + "Unable to create Direct3D Device.");
        return false;
    }
    // pointer for the backbuffer
    if(FAILED(m_lpD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_lpBackBuffer)))
    {
        // can´t create backbuffer
        ERROR_LOG(m_sLogLocationName + "Unable to create Direct3D Backbuffer.");
        return false;
    }

    m_lpD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
    return true;
}

void CDirect3D::BeginScene(void)
{
    if (!m_lpD3DDevice)
        return;

    m_lpD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, m_ClearColor, 0, 0);

    m_lpD3DDevice->BeginScene();
}

HRESULT CDirect3D::EndScene(void)
{
    if (!m_lpD3DDevice)
        return S_FALSE;

    m_lpD3DDevice->EndScene();
    m_uiSpriteIndex = 0;

    return m_lpD3DDevice->Present(0, 0, 0, 0);
}

HRESULT CDirect3D::ResetDevice(HWND hWnd, const UINT uiScreenWidth, const UINT uiScreenHeight, const bool bWindowed)
{
    m_PParams.hDeviceWindow    = hWnd;
    m_PParams.Windowed         = bWindowed;

    m_PParams.BackBufferWidth  = uiScreenWidth;
    m_PParams.BackBufferHeight = uiScreenHeight;

    // check if device is able to reset
    HRESULT hr = m_lpD3DDevice->TestCooperativeLevel();
    if (hr == D3DERR_DEVICENOTRESET)
    {
        // release backbuffer
        if(m_lpBackBuffer)
        {
            m_lpBackBuffer->Release();
            m_lpBackBuffer = NULL;
        }
        
        for (std::vector<LPD3DXSPRITE>::iterator itr = m_SpriteList.begin(); itr != m_SpriteList.end(); ++itr)
        {
            if (*itr)
                (*itr)->OnLostDevice();
        }

        // reset d3d device
        if (m_lpD3DDevice)
            hr = m_lpD3DDevice->Reset(&m_PParams);

        if (hr == S_OK)
        {
            for (std::vector<LPD3DXSPRITE>::iterator itr = m_SpriteList.begin(); itr != m_SpriteList.end(); ++itr)
            {
                if (*itr)
                    (*itr)->OnResetDevice();
            }

            // pointer for the backbuffer
            hr = m_lpD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_lpBackBuffer);
            if(hr == S_OK)
                BASIC_LOG(m_sLogLocationName + "Successfully recreate Direct3D Backbuffer.");
            else    // can´t create backbuffer
                ERROR_LOG(m_sLogLocationName + "Unable to recreate Direct3D Backbuffer.");
                
        }
    }

    return hr;
}

// release objects
void CDirect3D::CleanUp(void)
{
    ReleaseAllSprites();

    if(m_lpBackBuffer)
    {
        m_lpBackBuffer->Release();
        m_lpBackBuffer = NULL;
    }

    if(m_lpD3DDevice)
    {
        m_lpD3DDevice->Release();
        m_lpD3DDevice = NULL;
    }

    if(m_lpD3D)
    {
        m_lpD3D->Release();
        m_lpD3D = NULL;
    }
}

void CDirect3D::CreateSprite()
{
    LPD3DXSPRITE sprite;
    HRESULT hr = S_FALSE;
    hr = D3DXCreateSprite(m_lpD3DDevice, &sprite);

    if (hr == S_OK)
        m_SpriteList.push_back(sprite);
}

void CDirect3D::ReleaseAllSprites()
{
    for (std::vector<LPD3DXSPRITE>::iterator itr = m_SpriteList.begin(); itr != m_SpriteList.end(); ++itr)
    {
        if (*itr)
            (*itr)->Release();
    }
    m_SpriteList.clear();
}

void CDirect3D::DeleteOneSprite()
{
    if (m_SpriteList.empty())
        return;
}

LPD3DXSPRITE CDirect3D::GetSpriteForDraw(UINT DrawFlag)
{
    // add new sprite
    while(m_SpriteList.empty() || m_uiSpriteIndex > m_SpriteList.size() - 1)
        CreateSprite();

    if (!m_SpriteList.at(m_uiSpriteIndex))
        return NULL;

    m_SpriteList.at(m_uiSpriteIndex)->Begin(DrawFlag);
    return m_SpriteList.at(m_uiSpriteIndex);
}

void CDirect3D::EndSpriteDraw()
{
    if (m_uiSpriteIndex > m_SpriteList.size() - 1)
        return;

    if (m_SpriteList.at(m_uiSpriteIndex))
        m_SpriteList.at(m_uiSpriteIndex)->End();

    m_uiSpriteIndex++;
}