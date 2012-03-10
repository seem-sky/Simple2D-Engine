#include "DirectFont.h"
#include "Direct3D.h"

void DirectFont::DrawFont(std::string sText, RECT &pos, D3DXCOLOR color, UINT uiTextSize, UINT uiBoldness, std::string font, DWORD format, bool bItalic, LPD3DXSPRITE pSprite)
{
    LPD3DXFONT pFontObj = NULL;
    CDirect3D *pDirect3D = CDirect3D::Get();
    if (!pDirect3D)
        return;

    D3DXFONT_DESC FontDesc = {uiTextSize, 0, uiBoldness, 1, bItalic, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, ""};
    strcpy_s(FontDesc.FaceName, font.c_str());
    HRESULT hr = D3DXCreateFontIndirect(pDirect3D->GetDevice(), &FontDesc, &pFontObj);
    if (hr != S_OK)
        return;

    pFontObj->DrawText(pSprite, sText.c_str(), -1, &pos, format, color);

    // release
    if(pFontObj)
    {
        pFontObj->Release();
        pFontObj = NULL;
    }
}

DirectFont::DirectFont(UINT uiTextSize, UINT uiBoldness, std::string font, bool bItalic) : m_pFontObj(NULL)
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "DirectFont : ";

    CDirect3D *pDirect3D = CDirect3D::Get();
    if (!pDirect3D)
        return;

    D3DXFONT_DESC FontDesc = {uiTextSize, 0, uiBoldness, 1, bItalic, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_PITCH, ""};
    strcpy_s(FontDesc.FaceName, font.c_str());
    HRESULT hr = D3DXCreateFontIndirect(pDirect3D->GetDevice(), &FontDesc, &m_pFontObj);
    if (hr != S_OK)
        ERROR_LOG(m_sLogLocationName + "Unable to create FontObject.");
}

DirectFont::~DirectFont()
{
    // release
    if(m_pFontObj)
    {
        m_pFontObj->Release();
        m_pFontObj = NULL;
    }
}

void DirectFont::DrawFont(std::string sText, RECT &pos, D3DXCOLOR color, LPD3DXSPRITE pSprite, DWORD format)
{
    if (!m_pFontObj)
        return;

    m_pFontObj->DrawText(pSprite, sText.c_str(), -1, &pos, format, color);
}