#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <d3dx9.h>
#include "Logfile.h"
#include <vector>

class CDirect3D : public TSingleton<CDirect3D>
{
   public:
        CDirect3D();
        ~CDirect3D();

        bool Initialize(HWND hWnd, const UINT uiScreenWidth, const UINT uiScreenHeight, const bool bWindowed);
        void BeginScene(void);
        HRESULT EndScene(void);
        void SetClearColor(D3DCOLOR Color) { m_ClearColor = Color; }
        LPDIRECT3DDEVICE9 GetDevice(void) { return m_lpD3DDevice; }
        HRESULT ResetDevice(HWND hWnd, const UINT uiScreenWidth, const UINT uiScreenHeight, const bool bWindowed);

        /*#####
        ## sprites
        #####*/
        void CreateSprite();
        void ReleaseAllSprites();
        void DeleteOneSprite();
        LPD3DXSPRITE GetSpriteForDraw(UINT DrawFlag = D3DXSPRITE_ALPHABLEND);
        void EndSpriteDraw();
 
   protected:
       LPDIRECT3DSURFACE9 GetBackbuffer(void) { return m_lpBackBuffer; }
       void CleanUp(void);

       D3DCOLOR             m_ClearColor;                   // color of the clear backbuffer
       LPDIRECT3D9          m_lpD3D;
       LPDIRECT3DDEVICE9    m_lpD3DDevice;                  // direct3D device
       LPDIRECT3DSURFACE9   m_lpBackBuffer;                 // surface of the backbuffer

       D3DPRESENT_PARAMETERS m_PParams;

       std::vector<LPD3DXSPRITE> m_SpriteList;               // stores all sprites
       unsigned int m_uiSpriteIndex;
};
#endif