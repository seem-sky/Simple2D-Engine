#ifndef GAME_H
#define GAME_H

#include "Singleton.h"
#include "GameInfo.h"
#include "Logfile.h"
#include "Map.h"

// world class, add game code here
class WorldSession
{
public:
    WorldSession() { };
    virtual ~WorldSession() { };

    virtual void WorldUpdate(const UINT CurTime, const UINT CurElapsedTime) { }
};

class CGame : public TSingleton<CGame>
{
public:
    CGame(void);
    ~CGame(void);

    bool Initialize(HWND hWnd);
    bool Run(const UINT CurTime, const UINT CurElapsedTime);
    HRESULT Draw();
    void Quit();
    void SetWorldSession(WorldSession *pWorld) { m_pWorldSession = pWorld; }

    CGameInfo *GetGameInfo() { return &m_GameInfo; }

    HRESULT ResetDrawDevice(HWND hWnd);

private:
    CGameInfo m_GameInfo;
    bool Test;

    Map *m_pMap;

protected:
    CDirect3D *m_pDirect3D;
    WorldSession *m_pWorldSession;
};
#endif;