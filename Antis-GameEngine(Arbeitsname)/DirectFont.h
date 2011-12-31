#ifndef DIRECT_FONT_H
#define DIRECT_FONT_H

#include <d3dx9.h>
#include <string>

class DirectFont
{
public:
    static void DrawFont(std::string sText, RECT pos, D3DXCOLOR color, UINT uiTextSize, UINT uiBoldness,
        std::string font, DWORD format = DT_CENTER, bool bItalic = false);
};
#endif;