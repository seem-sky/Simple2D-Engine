#include "Game.h"
#include "RessourceManager.h"

CGame::CGame(void) : TSingleton()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "CGame : ";
    m_pLogfile          = CLogfile::Get();
    m_pDirect3D         = CDirect3D::Get();
    m_pWorldSession     = NULL;

    // read game ini data
    if (!m_GameInfo.ReadFile(GAME_DATA_GAME_INI))
        m_GameInfo.CreateIniByDefault();
    else
       BASIC_LOG(m_sLogLocationName + "Read and interpret " + GAME_DATA_GAME_INI + ".");
}

CGame::~CGame(void)
{
    Quit();
}

bool CGame::Initialize(HWND hWnd)
{
    // create and init direct3D
    m_pDirect3D = CDirect3D::Get();
    if (!m_pDirect3D)
    {
        ERROR_LOG(m_sLogLocationName + "Unable to create Direct3D.");
        return false;
    }

    unsigned int ScreenWidth, ScreenHeight;
    GetGameInfo()->GetWindowSize(ScreenWidth, ScreenHeight);
    if (!m_pDirect3D->Initialize(hWnd, ScreenWidth, ScreenHeight, GetGameInfo()->IsWindowed()))
    {
        ERROR_LOG(m_sLogLocationName + "Unable to initialize Direct3D.");
        return false;
    }

    m_pDirect3D->SetClearColor(D3DCOLOR_XRGB(0, 0, 0));
    BASIC_LOG(m_sLogLocationName + "Succesfully initialize Direct3D.");

    CRessourceManager::Get();

    // init ObjectLayer
    m_pLayerList.push_back(new MapLayer());
    ((MapLayer*)(*m_pLayerList.begin()))->LoadNewMap("Map4.map");
    //m_pLayerList.push_back(new ObjectLayer());

    return true;
}

bool CGame::Run(const UINT CurTime, const UINT CurElapsedTime)
{
    // Update all Layers
    for (LayerList::const_iterator itr = m_pLayerList.begin(); itr != m_pLayerList.end(); ++itr)
        (*itr)->UpdateLayer(CurTime, CurElapsedTime);

    // Update World
    if (m_pWorldSession)
        m_pWorldSession->WorldUpdate(CurTime, CurElapsedTime);

    return true;
}

void CGame::Draw()
{
    m_pDirect3D->BeginScene();
    // Draw all Layers
    for (LayerList::const_iterator itr = m_pLayerList.begin(); itr != m_pLayerList.end(); ++itr)
        (*itr)->DrawLayer();
    m_pDirect3D->EndScene();
}

void CGame::Quit()
{
    // release WorldSession
    if (m_pWorldSession)
        delete m_pWorldSession;

    // release Direct3D
    if (m_pDirect3D)
        m_pDirect3D->Del();

    // release ressource manager
    if (CRessourceManager *pRManager = CRessourceManager::Get())
        pRManager->Del();

    // release all layers
    for (LayerList::iterator itr = m_pLayerList.begin(); itr != m_pLayerList.end(); ++itr)
        delete (*itr);
    m_pLayerList.clear();
}


//######
//# class WorldSession
//######
WorldSession::WorldSession()
{
}

WorldSession::~WorldSession()
{
}

