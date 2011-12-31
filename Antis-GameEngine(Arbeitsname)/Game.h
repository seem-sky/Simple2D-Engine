#ifndef GAME_H
#define GAME_H

#include "GameInfo.h"
#include "Map.h"
#include "GameDatabase.h"
#include "DirectInput.h"
#include "Player.h"
#include "Menu.h"

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

    inline void CloseGame() { m_bGameClose = true; }

    // game pause
    inline void PauseGame(bool pause = true) { m_bPauseGame = pause; }
    inline bool IsGamePaused() { return m_bPauseGame; }

    CGameInfo *GetGameInfo() { return &m_GameInfo; }

    HRESULT ResetD3DXDevice(HWND hWnd);

    // menu
    void DisplayMenu(Menu *pMenu);
    void ShutDownMenu();
    inline bool ShowsMenu() { return m_pShownMenu ? true : false; }
    inline Menu* GetShownMenu() { return m_pShownMenu; }

private:
    CGameInfo m_GameInfo;
    bool Test;

    bool m_bGameClose;
    bool m_bPauseGame;

    Map *m_pMap;
    CDirect3D *m_pDirect3D;
    DirectInput *m_pDirectInput;
    WorldSession *m_pWorldSession;
    GameDatabase *m_pDatabase;

    // the main menu
    Menu *m_pShownMenu;

    PlayerPtrList PlayerList;

    Player* pPlayer;
};
#endif;