#include "Game.h"
#include "RessourceManager.h"

CGame::CGame(void) : TSingleton()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "CGame : ";
    m_pDirect3D         = NULL;
    m_pDirectInput      = NULL;
    m_pWorldSession     = NULL;
    m_pMap              = NULL;
    m_pDatabase         = NULL;

    // read game ini data
    if (!m_GameInfo.ReadFile(GAME_DATA_GAME_INI))
        m_GameInfo.CreateIniByDefault();
    else
       BASIC_LOG(m_sLogLocationName + "Read and interpret " + GAME_DATA_GAME_INI + ".");

    Test = false;

    pPlayer = new Player();
}

CGame::~CGame(void)
{
    Quit();
}

GAMEINIT_STATE CGame::Initialize(HINSTANCE hInstance, HWND hWnd)
{
    // create and init direct3D
    if (!m_pDirect3D)
    {
        m_pDirect3D = CDirect3D::Get();
        if (!m_pDirect3D)
        {
            ERROR_LOG(m_sLogLocationName + "Unable to create Direct3D.");
            return GAMEINIT_STATE_FAILED;
        }

        UINT ScreenWidth = 0, ScreenHeight = 0;
        GetGameInfo()->GetWindowSize(ScreenWidth, ScreenHeight);
        if (!m_pDirect3D->Initialize(hWnd, ScreenWidth, ScreenHeight, GetGameInfo()->IsWindowed()))
        {
            ERROR_LOG(m_sLogLocationName + "Unable to initialize Direct3D.");
            return GAMEINIT_STATE_FAILED;
        }

        m_pDirect3D->SetClearColor(D3DCOLOR_XRGB(0, 0, 0));
        BASIC_LOG(m_sLogLocationName + "Succesfully initialize Direct3D.");
    }

    // create an init DirectInput
    if(!m_pDirectInput)
    {
        m_pDirectInput = DirectInput::Get();
        if (m_pDirectInput)
        {
            m_pDirectInput->Init(hInstance, hWnd);
        }
        else
        {
            ERROR_LOG(m_sLogLocationName + "Unable to initialize DirectInput.");
            return GAMEINIT_STATE_FAILED;
        }

    }

    // create new map if not done
    if (!m_pMap) 
        m_pMap = new Map();

    // load complete database
    if (!m_pDatabase)
    {
        m_pDatabase = GameDatabase::Get();
        if (!m_pDatabase)
        {
            ERROR_LOG(m_sLogLocationName + "Unable to initialize Direct3D.");
            return GAMEINIT_STATE_FAILED;
        }
    }

    switch(m_pDatabase->LoadDatabase(GetGameInfo()->GetDatabaseLocation()))
    {
    case DATABASE_LOAD_RESULT_OK:
        BASIC_LOG(m_sLogLocationName + "Successfully load Game-Database.");
        return GAMEINIT_STATE_OK;

    case DATABASE_LOAD_RESULT_IN_PROGRESS:
        return GAMEINIT_STATE_IN_PROGRESS;

    case DATABASE_LOAD_RESULT_NO_FILE:
        ERROR_LOG(m_sLogLocationName + "Unable to load Game-Database." + GetGameInfo()->GetDatabaseLocation() + " no such file or directory.");
        return GAMEINIT_STATE_FAILED;

    case DATABASE_LOAD_RESULT_CORRUPT_FILE:
        ERROR_LOG(m_sLogLocationName + "Unable to load Game-Database." + GetGameInfo()->GetDatabaseLocation() + " is a corrupt file.");
        return GAMEINIT_STATE_FAILED;

    case DATABASE_LOAD_RESULT_FAILED:
    default:
        ERROR_LOG(m_sLogLocationName + "Unable to load Game-Database. Undefined error!");
        return GAMEINIT_STATE_FAILED;
    }
}

bool CGame::Run(const UINT CurTime, const UINT CurElapsedTime)
{
    if (!Test)
    {
        if (MAP_RESULT_DONE == m_pMap->LoadNewMap("Map1.map"))
        {
            pPlayer->SetControledUnit((Unit*)m_pMap->AddNewWorldObject(1, 100, 100, 0));
            Test = true;
        }
    }
    else
    {
        // update direct input
        m_pDirectInput->SetKeyStateKeyboard();

        // Update players
        if (pPlayer)
            pPlayer->UpdatePlayer(CurTime, CurElapsedTime);

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
    if (!PlayerList.empty())
    {
        for (PlayerPtrList::iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
            delete *itr;
    }
    // release WorldSession
    if (m_pWorldSession)
        delete m_pWorldSession;

    // release Direct3D
    if (m_pDirect3D)
        m_pDirect3D->Del();

    // release DirectInput
    if (m_pDirectInput)
        m_pDirectInput->Del();

    // release ressource manager
    if (CRessourceManager *pRManager = CRessourceManager::Get())
        pRManager->Del();

    // delete map
    if (m_pMap)
        delete m_pMap;
}

HRESULT CGame::ResetD3DXDevice(HWND hWnd)
{
    if (!m_pDirect3D)
        return S_FALSE;

    unsigned int xSize = 0, ySize = 0;
    m_GameInfo.GetWindowSize(xSize, ySize);

    return m_pDirect3D->ResetDevice(hWnd, xSize, ySize, m_GameInfo.IsWindowed());
}