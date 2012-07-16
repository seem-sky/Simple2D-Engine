#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include "Point.h"
#include "Texture.h"
#include "DirectFont.h"
#include "WrapperFunctions.h"
#include <vector>
#include <list>

struct TextChoice
{
    TextChoice(std::string sText)
    {
        m_sText = sText;
    }

    std::string m_sText;
};

typedef std::list<TextChoice> ChoiceList;

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
            if (m_pTexture->m_TextureInfo.m_uiSpriteType == (UINT)DATABASE::SPRITE_TYPE_TEXTBOX)
                return m_pTexture->m_TextureInfo.Type.Textbox.m_uiBorderSize;
        }
        return 0;
    }

    void SetTexture(TextureSource* pTexture);
    void SetTexture(UINT uiTextureID);

    void DrawTextbox();

    void Update(const ULONGLONG time, const UINT uiDiff);

    void SetAnimation(Point<int> moveTo, UINT uiTimeInMsec);

    bool AnimationFinished();
    bool AllLettersShown();
    inline bool IsScrolling() { return m_bIsScrolling; }
    inline bool CanScroll() { return m_bScrollText; }

    bool CanUsed();
    void Use();

    void MoveTextToLine(UINT uiLine, UINT uiTimePerLine);

protected:
    virtual void DrawTextboxPic();
    virtual void DrawTextboxMsg();
    virtual void DrawTextboxChoices();

    void ConvertMsg(std::string sText);
    void ShowAllLettersOnPage();

    std::string m_sLogLocationName;

private:
    Point<int> m_Position;
    Point<UINT> m_uiSize;

    ChoiceList m_ChoiceLIST;

    DirectFont m_DirectFont;

    std::vector<std::string> m_TextMsg;
    TextureSource* m_pTexture;

    bool m_bScrollText;
    bool m_bFastScroll;

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
    int m_MsgYPos;
    int m_NewMsgYPos;
    int m_StartMsgYPos;
    UINT m_uiCurRow;
    UINT m_uiCurRowShown;
    UINT m_uiScrollTime;
    UINT m_uiScroll_Timer;
    bool m_bIsScrolling;

    // page
    UINT m_uiMaxRowsShown;
    bool m_bNextPage;
    UINT m_uiCurPage;
    bool m_bOwnChoicePage;
};
#endif