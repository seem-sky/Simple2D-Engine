#ifndef GAME_H
#define GAME_H

#include "Singleton.h"
#include "GameInfo.h"
#include "Logfile.h"
#include "ObjectLayer.h"

typedef std::list<Layer*> LayerList;

class CGame : public TSingleton<CGame>
{
public:
    CGame(void);
    ~CGame(void);

    bool Initialize(HWND hWnd);
    bool Run(const UINT CurTime, const UINT CurElapsedTime);
    void Draw();
    void Quit();

    CGameInfo *GetGameInfo() { return &m_GameInfo; }

private:
    CGameInfo m_GameInfo;
    LayerList m_pLayerList;
    bool Test;

protected:
    CLogfile *m_pLogfile;
    CDirect3D *m_pDirect3D;
};

#endif;