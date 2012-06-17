#ifndef GAME_H
#define GAME_H

#include "GameInfo.h"
#include "Map.h"
#include "DirectInput.h"
#include "Player.h"
#include "Menu.h"
#include "TextBox.h"
#include "Database.h"

// world class, add game code here
class WorldSession
{
public:
    WorldSession() { };
    virtual ~WorldSession() { };

    virtual void WorldUpdate(const ULONGLONG CurTime, const UINT CurElapsedTime) { }
};

enum GAMEINIT_STATE
{
    GAMEINIT_STATE_OK,
    GAMEINIT_STATE_IN_PROGRESS,
    GAMEINIT_STATE_FAILED,
};

enum GAME_STATE
{
    GAME_NONE,
    GAME_RUN,
    GAME_LOAD_NEW_GAME,
    GAME_LOAD_GAME,
    GAME_PAUSE,
    GAME_END,
};

typedef std::list<Player*> PlayerPtrList;

class CGame : public TSingleton<CGame>
{
public:
    CGame(void);
    ~CGame(void);

    GAMEINIT_STATE Initialize(HINSTANCE hInstance, HWND hWnd);
    bool Run(const ULONGLONG CurTime, const UINT CurElapsedTime);
    HRESULT Draw();
    void Quit();
    void SetWorldSession(WorldSession *pWorld) { m_pWorldSession = pWorld; }

    inline void CloseGame() { m_GameState = GAME_END; }

    // game pause
    inline void PauseGame(bool pause = true) { pause ? m_GameState = GAME_PAUSE : m_GameState = GAME_RUN; }
    inline bool IsGamePaused() { return m_GameState == GAME_PAUSE ? true : false; }

    GameInfo *GetGameInfo() { return &m_GameInfo; }

    HRESULT ResetD3DXDevice(HWND hWnd);

    // menu
    void DisplayMenu(Menu *pMenu);
    void ShutDownMenu();
    inline bool ShowsMenu() { return m_pShownMenu ? true : false; }
    inline Menu* GetShownMenu() { return m_pShownMenu; }

    // textbox
    TextBox* ShowTextbox(TextBox* pBox);
    TextBox* ShowTextbox(std::string sMsg, UINT uiTextureID, Point<int> pos, Point<UINT> size, USHORT uiFontSize, USHORT uiBold, bool bItalic,
                            std::string sFont, ShowLetterTime showLetter, bool ScrollAble);
    // calc textbox size by screen size
    TextBox* ShowTextbox(std::string sMsg, UINT uiTextureID, Point<int> pos, USHORT uiFontSize, USHORT uiBold, bool bItalic,
                            std::string sFont, ShowLetterTime showLetter, bool ScrollAble);
    void ShutDownTextbox();
    inline bool ShowsTextbox() { return m_pShownTextBox ? true : false; }
    inline TextBox* GetShownTextbox() { return m_pShownTextBox; }

private:
    GAME_STATE m_GameState;
    GameInfo m_GameInfo;

    Map *m_pMap;
    CDirect3D *m_pDirect3D;
    DirectInput *m_pDirectInput;
    WorldSession *m_pWorldSession;
    DATABASE::Database *m_pGameDB;

    // the main menu
    Menu *m_pShownMenu;

    PlayerPtrList PlayerList;

    Player* pPlayer;

    TextBox* m_pShownTextBox;

    void CreateNewGame();
    void InitNewGame();
};
#endif