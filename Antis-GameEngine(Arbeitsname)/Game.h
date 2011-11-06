#ifndef GAME_H
#define GAME_H

#include "Singleton.h"
#include "GameInfo.h"
#include "Logfile.h"
#include "ObjectLayer.h"
#include "MapLayer.h"

enum DrawResult
{
    DRAW_RESULT_OK,
    DRAW_RESULT_DEVICE_LOST,
    DRAW_RESULT_BROKEN_POINTER,
};

typedef std::list<Layer*> LayerList;

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
    DrawResult Draw();
    void Quit();
    void SetWorldSession(WorldSession *pWorld) { m_pWorldSession = pWorld; }

    CGameInfo *GetGameInfo() { return &m_GameInfo; }

    DrawResult ResetDrawDevice(HWND hWnd);

private:
    CGameInfo m_GameInfo;
    LayerList m_pLayerList;
    bool Test;

protected:
    CLogfile *m_pLogfile;
    CDirect3D *m_pDirect3D;
    WorldSession *m_pWorldSession;
};
#endif;