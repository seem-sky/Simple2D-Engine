#include "Game.h"
#include "ResourceManager.h"
#include "DirectFont.h"

CGame::CGame(void) : m_pDirect3D(NULL), m_pDirectInput(NULL), m_pWorldSession(NULL), m_pMap(NULL), m_pGameDB(NULL), m_pShownMenu(NULL),
                m_pShownTextBox(NULL), m_GameState(GAME_NONE), TSingleton()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "CGame : ";

    m_pPlayer = new Player();
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

        Point<UINT> ScreenSize = GetGameInfo()->GetWindowSize();
        
        if (!m_pDirect3D->Initialize(hWnd, ScreenSize.x, ScreenSize.y, GetGameInfo()->IsWindowed()))
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
    if (!m_pGameDB)
    {
        m_pGameDB = DATABASE::Database::Get();
        if (!m_pGameDB)
        {
            ERROR_LOG(m_sLogLocationName + "Unable to initialize Game Database.");
            return GAMEINIT_STATE_FAILED;
        }

        m_pGameDB->LoadDB(GetGameInfo()->GetDatabaseLocation());
    }

    // check DB state
    switch(m_pGameDB->GetDBState())
    {
    case XML_Reader::XML_DONE:
        BASIC_LOG(m_sLogLocationName + "Successfully load Game-Database.");
        return GAMEINIT_STATE_OK;
        break;

    case XML_Reader::XML_IN_PROGRESS:
        return GAMEINIT_STATE_IN_PROGRESS;

    case XML_Reader::XML_NO_FILE:
        ERROR_LOG(m_sLogLocationName + "Unable to load Game-Database." + GetGameInfo()->GetDatabaseLocation() + " no such file or directory.");
        return GAMEINIT_STATE_FAILED;

    case XML_Reader::XML_CORRUPT_FILE:
        ERROR_LOG(m_sLogLocationName + "Unable to load Game-Database." + GetGameInfo()->GetDatabaseLocation() + " is a corrupt file.");
        return GAMEINIT_STATE_FAILED;

    case XML_Reader::XML_FAILED:
    default:
        ERROR_LOG(m_sLogLocationName + "Unable to load Game-Database. Undefined error!");
        return GAMEINIT_STATE_FAILED;
    }
}

bool CGame::Run(const ULONGLONG CurTime, const UINT CurElapsedTime)
{
    // do game events
    for (EventLIST::iterator t_EventItr = m_GameEvents.begin(); t_EventItr != m_GameEvents.end(); ++t_EventItr)
        if (!DoEventAction(&(*t_EventItr)))
            return false;

    m_GameEvents.clear();

    switch(m_GameState)
    {
    case GAME_NONE:
        CreateNewGame();
        break;

    case GAME_RUN:
    case GAME_PAUSE:
        // update direct input
        if (!m_pDirectInput->GetInput())
            PauseGame();

        // Update players
        if (m_pPlayer)
            m_pPlayer->UpdatePlayer(CurTime, CurElapsedTime);

        if (m_pShownMenu)
            m_pShownMenu->UpdateMenu(CurTime, CurElapsedTime);

        if (m_pShownTextBox)
            m_pShownTextBox->Update(CurTime, CurElapsedTime);

        // if no Textbox and no menu shown or if cur menu allows updating
        if (!m_pShownTextBox && (!m_pShownMenu || m_pShownMenu->ShouldUpdating()))
        {
            if (!IsGamePaused())
            {
                // Update World
                if (m_pWorldSession)
                    m_pWorldSession->WorldUpdate(CurTime, CurElapsedTime);

                // Update Map
                if (m_pMap)
                    m_pMap->UpdateMap(CurTime, CurElapsedTime);
            }
        }
        break;
    case GAME_LOAD_NEW_GAME:
        if (m_pMap->LoadNewMap(1) == MAP_RESULT_DONE)
            InitNewGame();
        break;
    case GAME_MAP_CHANGE:
        if (m_pMap->LoadNewMap(1) == MAP_RESULT_DONE)
            InitNewGame();
        break;
    case GAME_LOAD_GAME:
        break;
    }

    return true;
}

HRESULT CGame::Draw()
{
    m_pDirect3D->BeginScene();
    // Draw all Layers
    if (m_GameState == GAME_RUN || m_GameState == GAME_PAUSE)
    {
        if (!m_pShownMenu || m_pShownMenu->ShouldDrawMap())
        {
            if (m_pMap)
                m_pMap->Draw();

            if (m_pShownTextBox)
                m_pShownTextBox->DrawTextbox();

            // if pause is pressed, show a font on screen
            if (IsGamePaused())
            {
                Point<UINT> WindowSize = m_GameInfo.GetWindowSize();
                RECT rect = { 0, WindowSize.y / 2 -74, WindowSize.x, WindowSize.y };
                DirectFont::DrawFont("Pause", rect, D3DXCOLOR(0.25f, 0.4f, 1, 1), 74, 1000, "Comic Sans MS");
            }
        }

        if (m_pShownMenu)
            m_pShownMenu->DrawMenu();
    }

    return m_pDirect3D->EndScene();
}

void CGame::Quit()
{
    if (m_pShownMenu)
        m_pShownMenu->CloseAllMenus();

    // release WorldSession
    if (m_pWorldSession)
        delete m_pWorldSession;

    // release Direct3D
    if (m_pDirect3D)
        m_pDirect3D->Del();

    // release DirectInput
    if (m_pDirectInput)
        m_pDirectInput->Del();

    // release resource manager
    if (ResourceMgr *pResMgr = ResourceMgr::Get())
        pResMgr->Del();

    // delete map
    if (m_pMap)
        delete m_pMap;
}

HRESULT CGame::ResetD3DXDevice(HWND hWnd)
{
    if (!m_pDirect3D)
        return S_FALSE;

    Point<UINT> windowSize = m_GameInfo.GetWindowSize();

    return m_pDirect3D->ResetDevice(hWnd, windowSize.x, windowSize.y, m_GameInfo.IsWindowed());
}

void CGame::DisplayMenu(Menu *pMenu)
{
    if (!pMenu)
        return;

    pMenu->OnMenuOpen();
    m_pShownMenu = pMenu;
}

void CGame::ShutDownMenu()
{
    if (m_pShownMenu)
    {
        Menu* pMenu = m_pShownMenu->GetParentMenu();
        m_pShownMenu->CloseMenu();
        m_pShownMenu = pMenu;
    }
}

TextBox* CGame::ShowTextbox(TextBox *pBox)
{
    if (pBox)
        m_pShownTextBox = pBox;

    return pBox;
}

TextBox* CGame::ShowTextbox(std::string sMsg, UINT uiTextureID, Point<int> pos, Point<UINT> size, USHORT uiFontSize, USHORT uiBold, bool bItalic,
                            std::string sFont, ShowLetterTime showLetter, bool ScrollAble)
{
    return ShowTextbox(new TextBox(sMsg, pos, size, uiTextureID, uiFontSize, uiBold, bItalic, sFont, showLetter, ScrollAble));
}

TextBox* CGame::ShowTextbox(std::string sMsg, UINT uiTextureID, Point<int> pos, USHORT uiFontSize, USHORT uiBold, bool bItalic,
                            std::string sFont, ShowLetterTime showLetter, bool ScrollAble)
{
    Point<UINT> ScreenSize = m_GameInfo.GetWindowSize();
    Point<UINT> size(ScreenSize.x - (2*pos.x), ScreenSize.y - (pos.y + pos.x));
    return ShowTextbox(sMsg, uiTextureID, pos, size, uiFontSize, uiBold, bItalic, sFont, showLetter, ScrollAble);
}

void CGame::ShutDownTextbox()
{
    if (m_pShownTextBox)
    {
        delete m_pShownTextBox;
        m_pShownTextBox = NULL;
    }
}

void CGame::CreateNewGame()
{
    if (!m_pGameDB)
        return;

    DATABASE::StartConditionsPrototype t_StartProto;
    if (!m_pGameDB->GetStartConditions(t_StartProto))
        return;

    m_pMap->LoadNewMap(t_StartProto.m_uiMapID);

    m_GameState = GAME_LOAD_NEW_GAME;
}

void CGame::InitNewGame()
{
    if (!m_pGameDB)
        return;

    DATABASE::StartConditionsPrototype t_StartProto;
    if (!m_pGameDB->GetStartConditions(t_StartProto))
        return;

    Unit *pWho = (Unit*)m_pMap->AddNewWorldObject(1, t_StartProto.m_uiStartPos.x, t_StartProto.m_uiStartPos.y, 0);
    m_pPlayer->SetControledUnit(pWho);

    m_GameState = GAME_RUN;
}

bool CGame::DoEventAction(GameEvent *p_pEvent)
{
    if (!p_pEvent)
        return true;

    switch(p_pEvent->m_EventType)
    {
    case EVENT_MAP_CHANGE:
        if (!m_pMap)
            return true;

        m_pMap->ClearMap();
        m_pMap->LoadNewMap(p_pEvent->EventType.MapChange.m_uiMapID);
        m_GameState = GAME_MAP_CHANGE;
        break;

    case EVENT_CLOSE_GAME:
        return false;

    case EVENT_NONE:
    default:
        break;
    }

    return true;
}