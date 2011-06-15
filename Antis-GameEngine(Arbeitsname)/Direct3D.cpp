#include "Direct3D.h"


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

    D3DPRESENT_PARAMETERS PParams;
    ZeroMemory(&PParams, sizeof(PParams));

    PParams.SwapEffect       = D3DSWAPEFFECT_DISCARD;
    PParams.hDeviceWindow    = hWnd;
    PParams.Windowed         = bWindowed;

    PParams.BackBufferWidth  = uiScreenWidth;
    PParams.BackBufferHeight = uiScreenHeight;

    PParams.BackBufferFormat = D3DFMT_X8R8G8B8;

    PParams.Flags            = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    // create direct3D device
    if(FAILED(m_lpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &PParams, &m_lpD3DDevice)))
    {
        // can´t create direct3D device
        BASIC_LOG(m_sLogLocationName + "Unable to create Direct3D Device.");
        return false;
    }
    // pointer for the backbuffer
    if(FAILED(m_lpD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_lpBackBuffer)))
    {
        // can´t create backbuffer
        BASIC_LOG(m_sLogLocationName + "Unable to create Direct3D Backbuffer.");
        return false;
    }
    return true;
}

void CDirect3D::BeginScene(void)
{
    m_lpD3DDevice->Clear(0, 0, D3DCLEAR_TARGET, m_ClearColor, 0, 0);

    m_lpD3DDevice->BeginScene();
}

void CDirect3D::EndScene(void)
{
    m_lpD3DDevice->EndScene();

    m_lpD3DDevice->Present(0, 0, 0, 0);
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
