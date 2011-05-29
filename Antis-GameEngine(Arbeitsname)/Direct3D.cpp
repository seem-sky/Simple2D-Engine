#include "Direct3D.h"


CDirect3D::CDirect3D()
{
	m_pSwapChain            = 0;
	m_pDevice               = 0;
    m_pDeviceContext        = 0;
	m_pRenderTargetView     = 0;
	m_depthStencilBuffer    = 0;
	m_depthStencilState     = 0;
	m_depthStencilView      = 0;
	m_rasterState           = 0;
}

CDirect3D::~CDirect3D()
{
    Shutdown();
}

// Direct3D initialisieren
bool CDirect3D::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hWnd, bool fullscreen, float screenDepth, float screenNear)
{
	// create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);
    return true;
}

void CDirect3D::Shutdown()
{
    // close and release all existing COM objects
    m_pSwapChain->Release();
    m_pDevice->Release();
    m_pDeviceContext->Release();
}

void CDirect3D::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
    
	// Clear the depth buffer.
	m_pDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void CDirect3D::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_pSwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_pSwapChain->Present(0, 0);
	}

	return;
}


ID3D11Device* CDirect3D::GetDevice()
{
	return m_pDevice;
}

ID3D11DeviceContext* CDirect3D::GetDeviceContext()
{
	return m_pDeviceContext;
}

void CDirect3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}