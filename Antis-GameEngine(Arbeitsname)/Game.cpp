#include "Game.h"
#include "RessourceManager.h"
#include "SpriteFiles.h"

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

    Test = false;
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

    // get all sprite names, stored in different xml files
    SpriteFiles *pSpriteFiles = SpriteFiles::Get();
    pSpriteFiles->LoadSpriteDataFromFile(SPRITE_TYPE_MAP);
    pSpriteFiles->LoadSpriteDataFromFile(SPRITE_TYPE_AUTOTILE);
    pSpriteFiles->LoadSpriteDataFromFile(SPRITE_TYPE_OBJECT);

    // init ObjectLayer
    m_pLayerList.push_back(new MapLayer());
    //m_pLayerList.push_back(new ObjectLayer());

    return true;
}

bool CGame::Run(const UINT CurTime, const UINT CurElapsedTime)
{
    if (!Test)
    {
        if (MAP_RESULT_DONE == ((MapLayer*)(*m_pLayerList.begin()))->LoadNewMap("Map2.map"))
            Test = true;
    }
    else
    {
        // Update all Layers
        for (LayerList::const_iterator itr = m_pLayerList.begin(); itr != m_pLayerList.end(); ++itr)
            (*itr)->UpdateLayer(CurTime, CurElapsedTime);

        // Update World
        if (m_pWorldSession)
            m_pWorldSession->WorldUpdate(CurTime, CurElapsedTime);
    }

    return true;
}

DrawResult CGame::Draw()
{
        m_pDirect3D->BeginScene();
        // Draw all Layers
        if (Test)
            for (LayerList::const_iterator itr = m_pLayerList.begin(); itr != m_pLayerList.end(); ++itr)
                (*itr)->DrawLayer();
        if (m_pDirect3D->EndScene() == S_OK)
            return DRAW_RESULT_OK;
        else
            return DRAW_RESULT_DEVICE_LOST;
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
        delete *itr;

    m_pLayerList.clear();
}

DrawResult CGame::ResetDrawDevice(HWND hWnd)
{
    if (!m_pDirect3D)
        return DRAW_RESULT_BROKEN_POINTER;

    unsigned int xSize = 0, ySize = 0;
    m_GameInfo.GetWindowSize(xSize, ySize);
    if (S_OK != m_pDirect3D->ResetDevice(hWnd, xSize, ySize, m_GameInfo.IsWindowed()))
        return DRAW_RESULT_DEVICE_LOST;

    // recreate sprite for all layers
    for (LayerList::iterator itr = m_pLayerList.begin(); itr != m_pLayerList.end(); ++itr)
    {
        if (*itr)
            (*itr)->CreateSprite();
    }

    return DRAW_RESULT_OK;
}