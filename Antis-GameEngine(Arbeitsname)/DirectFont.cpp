#include "DirectFont.h"
#include "Direct3D.h"

void DirectFont::DrawFont(std::string sText, RECT pos, D3DXCOLOR color, UINT uiTextSize, UINT uiBoldness, std::string font, DWORD format, bool bItalic)
{
    LPD3DXFONT m_pFontObj = NULL;
    CDirect3D *pDirect3D = CDirect3D::Get();
    if (!pDirect3D)
        return;

    D3DXFONT_DESC FontDesc = {uiTextSize, 0, uiBoldness, 1, bItalic, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, ""};
    strcpy_s(FontDesc.FaceName, font.c_str());
    HRESULT hr = D3DXCreateFontIndirect(pDirect3D->GetDevice(),&FontDesc,&m_pFontObj);
    if (hr != S_OK)
        return;

    m_pFontObj->DrawText(NULL, sText.c_str(), -1, &pos, format, color);

    // release
    if(m_pFontObj != NULL)
    {
        m_pFontObj->Release();
        m_pFontObj = NULL;
    }
}