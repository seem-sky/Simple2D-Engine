#ifndef DIRECT3D_H
#define DIRECT3D_H

// include the basic windows header files and the Direct3D header files
#include <d3d11.h>
#include <d3dx11.h>
#include "Singleton.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

class CDirect3D : public TSingleton<CDirect3D>
{
public:
	CDirect3D();
	~CDirect3D();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();

	void GetVideoCardInfo(char*, int&);

private:
    // global declarations
    IDXGISwapChain *m_pSwapChain;             // the pointer to the swap chain interface
    ID3D11Device *m_pDevice;                     // the pointer to our Direct3D device interface
    ID3D11DeviceContext *m_pDeviceContext;           // the pointer to our Direct3D device context
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
};
#endif