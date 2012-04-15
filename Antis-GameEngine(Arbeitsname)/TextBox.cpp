#include "TextBox.h"
#include "Logfile.h"
#include "Game.h"

TextBox::TextBox(std::string sMsg, Point<int> pos, Point<UINT> size, UINT uiTextureID, USHORT uiLetterSize, USHORT uiBold, bool bItalic, std::string sFont,
                 ShowLetterTime showLetter, bool ScrollAble)
: m_pTexture(NULL), m_Position(pos), m_uiSize(size), m_uiAniTime(0), m_uiStartAniTime(0), m_LetterTimer(showLetter),
m_uiShownLetterTime(showLetter), m_uiShownLetters(0), m_MsgYPos(0), m_NewMsgYPos(0), m_uiCurRow(0), m_uiScrollTime(0), m_uiScroll_Timer(0),
m_uiCurRowShown(0), m_StartMsgYPos(0), m_bScrollText(ScrollAble), m_bIsScrolling(false), m_bNextSide(false), m_DirectFont(uiLetterSize, uiBold, sFont, bItalic)
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "Textbox : ";
    if (uiTextureID)
        SetTexture(uiTextureID);
    ConvertMsg(sMsg);

    // show letter instant is useless for scrolling if text has more lines than can draw
    if (m_LetterTimer == SHOW_LETTER_INSTANT && CanScroll() && (m_uiSize.y - 2*GetTexturBorderSize()) / m_DirectFont.GetFontSize() < m_TextMsg.size())
    {
        m_LetterTimer = SHOW_LETTER_VERY_FAST;
        m_uiShownLetterTime = SHOW_LETTER_VERY_FAST;
    }

    m_MsgYPos = m_Position.y;
    UINT borderSize = GetTexturBorderSize();
    m_MsgYPos += borderSize;
    m_uiMaxRowsShown = (m_uiSize.y- 2*borderSize)/ m_DirectFont.GetFontSize();

    //m_ChoiceLIST.push_back(TextChoice("Test1"));
    //m_ChoiceLIST.push_back(TextChoice("Test2"));
    //m_ChoiceLIST.push_back(TextChoice("Test3"));
    //m_ChoiceLIST.push_back(TextChoice("Test4"));
}

TextBox::~TextBox(void)
{
}

void TextBox::DrawTextbox()
{
    DrawTextboxPic();
    if (m_uiStartAniTime == 0)
        DrawTextboxMsg();
}

void TextBox::DrawTextboxPic()
{
    if (!m_pTexture)
        return;

    CDirect3D *pDirect3D = CDirect3D::Get();
    if (!pDirect3D)
        return;

    LPD3DXSPRITE pSprite = pDirect3D->GetSpriteForDraw();

    Point<UINT> TextureSize;
    m_pTexture->GetTextureSize(TextureSize.x, TextureSize.y);

    UINT uiSrcRow = 0;
    RECT src = {0,0,0,0};

    D3DXVECTOR3 vCenter((float)(m_Position.x *2 + m_uiSize.x)/2, (float)(m_Position.y + m_Position.x + m_uiSize.y)/2, 0);
    for (UINT i = 0; i <= ceil((float)(m_uiSize.y / (TextureSize.y / 3))); i++)
    {
        float fYPos = (float)m_Position.y + i*(TextureSize.y/3);
        if (i > ceil((float)(m_uiSize.y / (TextureSize.y / 3))) -1)
        {
            fYPos -= TextureSize.y / 3 - m_uiSize.y % (TextureSize.y/3);
            uiSrcRow = 2;
        }
        else if (i >= 1)
            uiSrcRow = 1;

        src.left = 0;
        src.top = (TextureSize.x / 3)*uiSrcRow;
        src.right = TextureSize.x / 3;
        src.bottom = (TextureSize.y / 3)*(uiSrcRow+1);

        // draw left side
        pSprite->Draw(m_pTexture->m_pTexture, &src, NULL, &D3DXVECTOR3((float)m_Position.x, fYPos, 0), D3DXCOLOR(1,1,1,1));

        src.left = TextureSize.x / 3;
        src.right = (TextureSize.x / 3)*2;
        for (UINT j = 1; j <= ceil((float)(m_uiSize.x / (TextureSize.x / 3)))-1; j++)
            pSprite->Draw(m_pTexture->m_pTexture, &src, NULL, &D3DXVECTOR3((float)m_Position.x + j*(TextureSize.x/3), fYPos, 0), D3DXCOLOR(1,1,1,1));

        src.left = (TextureSize.x / 3)*2;
        src.right = (TextureSize.x / 3)*3;

        // draw right side
        pSprite->Draw(m_pTexture->m_pTexture, &src, NULL, &D3DXVECTOR3((float)m_Position.x + m_uiSize.x - TextureSize.x/3, fYPos, 0), D3DXCOLOR(1,1,1,1));
    }
    pDirect3D->EndSpriteDraw();
}

void TextBox::DrawTextboxMsg()
{
    if (m_TextMsg.empty())
        return;

    CDirect3D *pDirect3D = CDirect3D::Get();
    if (!pDirect3D)
        return;

    UINT uiBorderSize = GetTexturBorderSize();
    std::string sMsg;
    for (UINT uiRow = 0; uiRow <= m_uiCurRow; ++uiRow)
    {
        if (uiRow == m_uiCurRow)
            sMsg += m_TextMsg.at(uiRow).substr(0, m_uiShownLetters);
        else
            sMsg += m_TextMsg.at(uiRow);
    }

    RECT pos = {m_Position.x + uiBorderSize, m_Position.y + uiBorderSize, m_Position.x + m_uiSize.x - uiBorderSize, m_Position.y + m_uiSize.y - uiBorderSize };
    // set scissor rect to msgbox area
    RECT pRect = pos;

    // if not scrolling, set scissor rect to exactly line size
    if (!CanScroll())
        pRect.bottom = pRect.top + m_uiMaxRowsShown*m_DirectFont.GetFontSize();

    if (LPDIRECT3DDEVICE9 pDevice = pDirect3D->GetDevice())
        pDevice->SetScissorRect(&pRect);

    LPD3DXSPRITE pSprite = pDirect3D->GetSpriteForDraw();

    pos.top = m_MsgYPos;

    //// add all choice to sMsg
    //if (AllLettersShown() && !IsScrolling())
    //{
    //    for (ChoiceList::iterator itr = m_ChoiceLIST.begin(); itr != m_ChoiceLIST.end(); ++itr)
    //        sMsg += "\n" + itr->m_sText;
    //}

    m_DirectFont.DrawFont(sMsg, pos, D3DXCOLOR(0.5f, 0.4f, 0.6f, 1), pSprite, DT_LEFT);

    pDirect3D->EndSpriteDraw();
    // set scissor rect to window size
    if (CGame *pGame = CGame::Get())
    {
        if (GameInfo *pInfo = pGame->GetGameInfo())
        {
            Point<UINT> ScreenSize = pInfo->GetWindowSize();
            pRect.left = 0;
            pRect.top = 0;
            pRect.right = ScreenSize.x;
            pRect.bottom = ScreenSize.y;

            if (LPDIRECT3DDEVICE9 pDevice = pDirect3D->GetDevice())
                pDevice->SetScissorRect(&pRect);
        }
    }
}

void TextBox::SetTexture(UINT uiTextureID)
{
    if (TextureMgr *pMgr = TextureMgr::Get())
        SetTexture(pMgr->GetTextureSource(uiTextureID));
}

void TextBox::SetTexture(TextureSource *pTexture)
{
    if (!pTexture)
        return;

    if (pTexture->m_TextureInfo.m_uiSpriteType != SPRITE_TYPE_TEXTBOX)
    {
        ERROR_LOG(m_sLogLocationName + pTexture->m_TextureInfo.m_sFileName + " is not a valid texture for Textboxes. Texture not changed.");
        return;
    }

    m_pTexture = pTexture;
    BASIC_LOG(m_sLogLocationName + "Change texture with " + pTexture->m_TextureInfo.m_sFileName +".");
}

void TextBox::Update(const ULONGLONG time, const UINT uiDiff)
{
    // update transition
    if (m_uiStartAniTime != 0)
    {
        if (m_uiAniTime <= uiDiff)
        {
            m_Position = m_MoveTo;
            m_uiAniTime = 0;
            m_uiStartAniTime = 0;
            m_StartPos.x = 0;
            m_StartPos.y = 0;
            m_MoveTo.x = 0;
            m_MoveTo.y = 0;

            m_MsgYPos = m_Position.y;
            m_MsgYPos += GetTexturBorderSize();
        }
        else
        {
            m_uiAniTime -= uiDiff;
            UINT diff = m_uiStartAniTime - m_uiAniTime;
            m_Position.x = (int)(((float)(m_MoveTo.x - m_StartPos.x)/100)*(float)((m_uiStartAniTime - m_uiAniTime)*100/m_uiStartAniTime));
            m_Position.x += m_StartPos.x;
            m_Position.y = (int)(((float)(m_MoveTo.y - m_StartPos.y)/100)*(float)((m_uiStartAniTime - m_uiAniTime)*100/m_uiStartAniTime));
            m_Position.y += m_StartPos.y;
        }
    }
    else
    {
        // if side method and next side is set, return
        if (!CanScroll() && m_bNextSide)
            return;

        // update letters
        if (!AllLettersShown())
        {
            if (m_LetterTimer == SHOW_LETTER_INSTANT)
            {
                //UINT uiNCounter = 0;
                //while(uiNCounter < m_uiMaxRowsShown)
                //{
                //    // if is new line
                //    if (m_sTextMsg.at(m_uiShownLetters) == 10)
                //        uiNCounter++;

                //    m_uiShownLetters++;
                //    if (m_uiShownLetters >= m_sTextMsg.size())  
                //        return;
                //}
                //m_uiCurRow += uiNCounter;
                //m_bNextSide = true;
            }
            else
            {
                if (!IsScrolling())
                {
                    if (m_uiShownLetterTime < uiDiff)
                    {
                        m_uiShownLetterTime = m_LetterTimer;
                        m_uiShownLetters++;

                        // update rows
                        if (m_TextMsg.at(m_uiCurRow).size() <= m_uiShownLetters && m_TextMsg.size()-1 > m_uiCurRow)
                        {
                            m_uiShownLetters = 0;
                            m_uiCurRow++;
                            if (CanScroll())
                            {
                                if (m_uiSize.y / m_DirectFont.GetFontSize() <= (m_uiCurRow+2)-m_uiCurRowShown && m_uiCurRow < m_TextMsg.size()-1)
                                    MoveTextToLine(m_uiCurRowShown+1, 500);
                            }
                            else
                            {
                                if (m_uiCurRow - m_uiCurRowShown >= m_uiMaxRowsShown)
                                    m_bNextSide = true;
                            }
                        }

                        // if all letters shown and choice list not empty
                        if (AllLettersShown() && !m_ChoiceLIST.empty())
                        {
                            m_uiCurRow += 2;
                            m_uiCurRowShown += 2;
                            m_uiScrollTime += 1000;
                            m_uiScroll_Timer += m_uiScrollTime;
                            m_NewMsgYPos = m_MsgYPos - (2*m_DirectFont.GetFontSize());
                            m_StartMsgYPos = m_MsgYPos;
                        }
                    }
                    else
                        m_uiShownLetterTime -= uiDiff;      
                }
            }
        }

        // update scrolling
        if (IsScrolling())
        {
            if (m_uiScroll_Timer <= uiDiff)
            {
                m_MsgYPos = m_NewMsgYPos;
                m_uiScroll_Timer = 0;
                m_uiScrollTime = 0;
                m_StartMsgYPos = 0;
                m_NewMsgYPos = 0;
                m_bIsScrolling = false;
            }
            else
            {
                m_uiScroll_Timer -= uiDiff;
                UINT diff = m_uiScrollTime - m_uiScroll_Timer;
                m_MsgYPos = (int)(((float)(m_NewMsgYPos - m_StartMsgYPos)/100)*(float)((m_uiScrollTime - m_uiScroll_Timer)*100/m_uiScrollTime));
                m_MsgYPos += m_StartMsgYPos;
            }
        }
    }
}

void TextBox::SetAnimation(Point<int> moveTo, UINT uiTimeInMsec)
{
    m_StartPos = m_Position;
    m_MoveTo = moveTo;
    m_uiAniTime = uiTimeInMsec;
    m_uiStartAniTime = uiTimeInMsec;
}

bool TextBox::CanUsed()
{
    if (!AnimationFinished())
        return false;

    if (!AllLettersShown())
        return false;

    if (IsScrolling())
        return false;

    return true;
}

bool TextBox::AnimationFinished()
{
    if (m_uiStartAniTime)
        return false;

    return true;
}

bool TextBox::AllLettersShown()
{
    if (m_TextMsg.at(m_uiCurRow).size() <= m_uiShownLetters && m_TextMsg.size()-1 <= m_uiCurRow)
        return true;

    return false;
}

void TextBox::Use()
{
    if (!AnimationFinished())
        return;

    // if is side system
    if (!CanScroll())
    {
        //if (m_bNextSide)
        //{
        //    m_MsgYPos -= m_uiMaxRowsShown*m_DirectFont.GetFontSize();
        //    m_bNextSide = false;
        //    m_uiCurRow++;
        //    m_uiCurRowShown = m_uiCurRow;
        //    return;
        //}
        //else if (m_uiShownLetters < m_sTextMsg.size())
        //{
        //    UINT uiNCounter = m_uiCurRow-m_uiCurRowShown;
        //    while(uiNCounter < m_uiMaxRowsShown)
        //    {
        //        // if is new line
        //        if (m_sTextMsg.at(m_uiShownLetters) == 10)
        //            uiNCounter++;

        //        m_uiShownLetters++;
        //        if (m_uiShownLetters >= m_sTextMsg.size())  
        //            return;
        //    }
        //    m_uiCurRow += uiNCounter;
        //    m_bNextSide = true;
        //    return;
        //}
    }
    // if its scroll system
    else if (CanScroll() && !AllLettersShown())
    {
        m_uiShownLetters = m_TextMsg.at(m_TextMsg.size()-1).size();
        m_uiCurRow = m_TextMsg.size()-1;
        MoveTextToLine(m_uiCurRow+1 - (m_uiSize.y-2*GetTexturBorderSize()) / m_DirectFont.GetFontSize(), 0);
        return;
    }

    if (CGame *pGame = CGame::Get())
            pGame->ShutDownTextbox();
}

void TextBox::ConvertMsg(std::string sText)
{
    RECT size = {0,0,0,0};
    std::string sSub;
    UINT borderSize = GetTexturBorderSize();
    while(!sText.empty())
    {
        if (sText.find(10) < sText.size())
            sSub = sText.substr(0, sText.find(10)+1);
        else
            sSub = sText;

        m_DirectFont.DrawFont(sSub, size, D3DXCOLOR(1, 1, 1, 1), NULL, DT_CALCRECT);
        while ((UINT)size.right > m_uiSize.x - 2*borderSize)
        {
            // check if there is any space in string
            if (sSub.find(32) >= sSub.size())
                break;

            sSub = sSub.substr(0, sSub.find_last_of(32));
            m_DirectFont.DrawFont(sSub, size, D3DXCOLOR(1, 1, 1, 1), NULL, DT_CALCRECT);
        }
        
        if (!sSub.empty())
        {
            sText.erase(0, sSub.size());
            if (sSub.at(sSub.size()-1) != 10 && !sText.empty() && sText.at(0) == 32)
            {
                sSub.push_back(10);
                sText.erase(0,1);
            }
            m_TextMsg.push_back(sSub);
        }
    }
}

void TextBox::MoveTextToLine(UINT uiLine, UINT uiTimePerLine)
{
    int LineDiff = uiLine-m_uiCurRowShown;
    m_uiCurRowShown = uiLine;
    // set scroll time
    m_uiScrollTime = LineDiff > 0 ? uiTimePerLine*LineDiff : (-1)*uiTimePerLine*LineDiff;
    m_uiScroll_Timer = m_uiScrollTime;

    // set position
    m_NewMsgYPos = m_Position.y + GetTexturBorderSize() - (m_DirectFont.GetFontSize()*uiLine);
    m_StartMsgYPos = m_MsgYPos;

    m_bIsScrolling = true;
}