#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <d3dx9.h>
#include "Logfile.h"

class CDirect3D : public TSingleton<CDirect3D>
{
   public:
        CDirect3D();
        ~CDirect3D();

        bool Initialize(HWND hWnd, const UINT uiScreenWidth, const UINT uiScreenHeight, const bool bWindowed);
        void BeginScene(void);
        void EndScene(void);
        void CDirect3D::SetClearColor(D3DCOLOR Color) { m_ClearColor = Color; }
        LPDIRECT3DDEVICE9 CDirect3D::GetDevice(void) { return m_lpD3DDevice; }
 
   protected:
       LPDIRECT3DSURFACE9 GetBackbuffer(void) { return m_lpBackBuffer; }
       void CleanUp(void);

       D3DCOLOR             m_ClearColor;                   // color of the clear backbuffer
       LPDIRECT3D9          m_lpD3D;
       LPDIRECT3DDEVICE9    m_lpD3DDevice;                  // direct3D device
       LPDIRECT3DSURFACE9   m_lpBackBuffer;                 // surface of the backbuffer

       CLogfile *m_pLogfile;
};
#endif