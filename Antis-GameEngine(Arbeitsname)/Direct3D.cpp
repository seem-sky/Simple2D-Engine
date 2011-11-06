#include "Direct3D.h"
#include "RessourceManager.h"


CDirect3D::CDirect3D()
{
    m_lpD3D                         = NULL;
    m_lpD3DDevice                   = NULL;
    m_lpBackBuffer                  = NULL;
    m_pLogfile                      = CLogfile::Get();
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
    if(FAILED(m_lpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_PParams, &m_lpD3DDevice)))
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

    return m_lpD3DDevice->Present(0, 0, 0, 0);
}

HRESULT CDirect3D::ResetDevice(HWND hWnd, const UINT uiScreenWidth, const UINT uiScreenHeight, const bool bWindowed)
{
    m_PParams.hDeviceWindow    = hWnd;
    m_PParams.Windowed         = bWindowed;

    m_PParams.BackBufferWidth  = uiScreenWidth;
    m_PParams.BackBufferHeight = uiScreenHeight;

    if (CRessourceManager *pResMgr = CRessourceManager::Get())
    {
        pResMgr->ClearCharsetTextures();
        pResMgr->ClearTileTextures();
    }

    HRESULT hr = S_FALSE;

    if (m_lpD3DDevice)
        hr = m_lpD3DDevice->Reset(&m_PParams);

    if (hr != S_OK)
    {
        if (m_lpD3DDevice)
            m_lpD3DDevice->Release();

        m_lpD3DDevice = NULL;

        // recreate direct3D device
        hr = m_lpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_PParams, &m_lpD3DDevice);
        if(hr != S_OK)
            // can´t create direct3D device
            ERROR_LOG(m_sLogLocationName + "Unable to recreate Direct3D Device.");
    }

    return hr;
}

// release objects
void CDirect3D::CleanUp(void)
{
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
