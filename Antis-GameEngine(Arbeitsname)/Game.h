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
    GAME_MAP_CHANGE,
    GAME_PAUSE,
};

enum GameEventType
{
    EVENT_NONE,
    EVENT_MAP_CHANGE,
    EVENT_CLOSE_GAME,
};

struct GameEvent
{
    GameEvent() : m_EventType(EVENT_NONE)
    {
        memset(&EventType, 0, sizeof(EventType));
    }

    GameEventType m_EventType;

    union
    {
        struct sMapChange
        {
            UINT m_uiMapID;
            UINT m_uiNewPlayerPosX;
            UINT m_uiNewPlayerPosY;
        }MapChange;
    }EventType;

};

typedef std::list<GameEvent> EventLIST;

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

    // GameEvents
    inline void AddGameEvent(GameEvent t_newEvent) { m_GameEvents.push_back(t_newEvent); }
    inline void CloseGame()
    {
        GameEvent t_Event;
        t_Event.m_EventType = EVENT_CLOSE_GAME;
        AddGameEvent(t_Event);
    }

    // player
    Player* GetPlayer() { return m_pPlayer; }

private:
    GAME_STATE m_GameState;
    GameInfo m_GameInfo;

    Map *m_pMap;
    CDirect3D *m_pDirect3D;
    DirectInput *m_pDirectInput;
    WorldSession *m_pWorldSession;
    DATABASE::Database *m_pGameDB;

    EventLIST m_GameEvents;

    // the main menu
    Menu *m_pShownMenu;

    Player* m_pPlayer;

    TextBox* m_pShownTextBox;

    void CreateNewGame();
    void InitNewGame();

    bool DoEventAction(GameEvent *p_pEvent);
};
#endif