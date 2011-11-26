#include "Game.h"
#include "RessourceManager.h"
#include "SpriteFiles.h"

CGame::CGame(void) : TSingleton()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "CGame : ";
    m_pDirect3D         = CDirect3D::Get();
    m_pWorldSession     = NULL;
    m_pMap              = NULL;

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

    m_pMap = new Map();

    return true;
}

bool CGame::Run(const UINT CurTime, const UINT CurElapsedTime)
{
    if (!Test)
    {
        if (MAP_RESULT_DONE == m_pMap->LoadNewMap("Map2.map"))
            Test = true;
    }
    else
    {
        // Update Map
        if (m_pMap)
            m_pMap->UpdateMap(CurTime, CurElapsedTime);

        // Update World
        if (m_pWorldSession)
            m_pWorldSession->WorldUpdate(CurTime, CurElapsedTime);
    }

    return true;
}

HRESULT CGame::Draw()
{
    m_pDirect3D->BeginScene();
    // Draw all Layers
    if (Test && m_pMap)
        m_pMap->Draw();

    return m_pDirect3D->EndScene();
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

    // delete map
    if (m_pMap)
        delete m_pMap;
}

HRESULT CGame::ResetDrawDevice(HWND hWnd)
{
    if (!m_pDirect3D)
        return S_FALSE;

    unsigned int xSize = 0, ySize = 0;
    m_GameInfo.GetWindowSize(xSize, ySize);

    return m_pDirect3D->ResetDevice(hWnd, xSize, ySize, m_GameInfo.IsWindowed());
}