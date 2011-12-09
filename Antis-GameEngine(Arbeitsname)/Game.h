#ifndef GAME_H
#define GAME_H

#include "GameInfo.h"
#include "Map.h"
#include "GameDatabase.h"
#include "DirectInput.h"

// world class, add game code here
class WorldSession
{
public:
    WorldSession() { };
    virtual ~WorldSession() { };

    virtual void WorldUpdate(const UINT CurTime, const UINT CurElapsedTime) { }
};

enum GAMEINIT_STATE
{
    GAMEINIT_STATE_OK,
    GAMEINIT_STATE_IN_PROGRESS,
    GAMEINIT_STATE_FAILED,
};

class CGame : public TSingleton<CGame>
{
public:
    CGame(void);
    ~CGame(void);

    GAMEINIT_STATE Initialize(HINSTANCE hInstance, HWND hWnd);
    bool Run(const UINT CurTime, const UINT CurElapsedTime);
    HRESULT Draw();
    void Quit();
    void SetWorldSession(WorldSession *pWorld) { m_pWorldSession = pWorld; }

    CGameInfo *GetGameInfo() { return &m_GameInfo; }

    HRESULT ResetD3DXDevice(HWND hWnd);

private:
    CGameInfo m_GameInfo;
    bool Test;

    Map *m_pMap;
    CDirect3D *m_pDirect3D;
    DirectInput *m_pDirectInput;
    WorldSession *m_pWorldSession;
    GameDatabase *m_pDatabase;
};
#endif;