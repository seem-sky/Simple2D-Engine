#ifndef DIRECT_FONT_H
#define DIRECT_FONT_H

#include <d3dx9.h>
#include <string>
#include "Global.h"

class DirectFont
{
public:
    DirectFont(uint32 uiTextSize, uint32 uiBoldness, std::string font, bool bItalic);
    ~DirectFont();

    // draw string
    static void DrawFont(std::string sText, RECT &pos, D3DXCOLOR color, uint32 uiTextSize, uint32 uiBoldness,
        std::string font, DWORD format = DT_CENTER, bool bItalic = false, LPD3DXSPRITE pSprite = NULL);

    void DrawFont(std::string sText, RECT &pos, D3DXCOLOR color, LPD3DXSPRITE pSprite = NULL, DWORD format = DT_CENTER);

    inline USHORT GetFontSize()
    {
        if (!m_pFontObj)
            return 0;

        D3DXFONT_DESC FontDesc;
        m_pFontObj->GetDesc(&FontDesc);
        return FontDesc.Height;
    }

private:
    LPD3DXFONT m_pFontObj;
    std::string m_sLogLocationName;
};
#endif;