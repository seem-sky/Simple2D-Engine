#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "Point.h"
#include "Texture.h"
#include "DirectFont.h"

class TextBox
{
public:
    TextBox(std::string sMsg, Point<int> pos, Point<UINT> size, UINT uiTextureID, USHORT uiLetterSize, USHORT uiBold, bool bItalic, std::string sFont,
                ShowLetterTime showLetter = SHOW_LETTER_NORMAL, bool ScrollAble = true);
    virtual ~TextBox(void);

    inline Point<int> GetPosition() { return m_Position; }
    inline void SetPosition(Point<int> pos) { m_Position = pos; }

    inline Point<UINT> GetSize() { return m_uiSize; }
    inline UINT GetTexturBorderSize()
    {
        if (m_pTexture)
        {
            if (m_pTexture->m_TextureInfo.m_uiSpriteType == (UINT)SPRITE_TYPE_TEXTBOX)
                return m_pTexture->m_TextureInfo.Type.Textbox.m_uiBorderSize;
        }
        return 0;
    }

    void SetTexture(TextureSource* pTexture);
    void SetTexture(UINT uiTextureID);

    void DrawTextbox();

    void Update(const ULONGLONG time, const UINT uiDiff);

    void SetAnimation(Point<int> moveTo, UINT uiTimeInMsec);

    bool CanUsed();
    bool AnimationFinished();
    bool AllLettersShown();
    bool IsScrolling();

    void Use();
    inline bool CanScroll() { return m_bScrollText; }

protected:
    virtual void DrawTextboxPic();
    virtual void DrawTextboxMsg();

    void ConvertMsg();

    std::string m_sLogLocationName;

private:
    Point<int> m_Position;
    Point<UINT> m_uiSize;

    DirectFont m_DirectFont;

    std::string m_sTextMsg;
    TextureSource* m_pTexture;

    bool m_bScrollText;

    // animation
    Point<int> m_MoveTo;
    Point<int> m_StartPos;
    UINT m_uiAniTime;
    UINT m_uiStartAniTime;

    // letter animation
    UINT m_uiShownLetters;
    ShowLetterTime m_LetterTimer;
    UINT m_uiShownLetterTime;

    // scrolling
    UINT m_uiRows;
    int m_MsgYPos;
    int m_NewMsgYPos;
    int m_StartMsgYPos;
    UINT m_uiCurRow;
    UINT m_uiCurRowShown;
    UINT m_uiScrollTime;
    UINT m_uiScroll_Timer;

    // side
    UINT m_uiMaxRowsShown;
    bool m_bNextSide;
};
#endif;