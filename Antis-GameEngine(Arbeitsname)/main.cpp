#include <windows.h>
#include "Time.h"
#include "Game.h"

// globals
HINSTANCE g_hInst       = NULL;
HWND g_hWnd             = NULL;
CLogfile *m_pLogfile    = NULL;
CTime *m_pTime          = NULL;
CGame *m_pGame          = NULL;

HRESULT InitWindow ( HINSTANCE hInstance, int nCmdShow, CGameInfo *GameInfo );
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
void ReleaseObjects();

// the entry point for any Windows program
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    // start logfile
    m_pLogfile = CLogfile::Get();
    if(!m_pLogfile)
    {
        ReleaseObjects();
        return 0;
    }
    string m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "main : ";

    // create CGame and intepret game.ini
    m_pGame = CGame::Get();
    if (!m_pGame)
    {
        ERROR_LOG(m_sLogLocationName + "Unable to create Game.");
        ReleaseObjects();
        return 0;
    }

    // register and create window
    if( FAILED(InitWindow( hInstance, nCmdShow, m_pGame->GetGameInfo()) ) )
    {
        ERROR_LOG(m_sLogLocationName + "Unable to register WindowClass or create window.");
        ReleaseObjects();
        return 0;
    }
    else
        BASIC_LOG(m_sLogLocationName + "Succesfully create window.");

    // create and start CTime
    m_pTime = CTime::Get();
    if (!m_pTime)
    {
        ERROR_LOG(m_sLogLocationName + "Unable to create Time Object.");
        ReleaseObjects();
        return 0;
    }
    BASIC_LOG(m_sLogLocationName + "Succesfully start Time.");

    CoInitialize(NULL);

   // enter the main loop:
    MSG msg = {0};
    HRESULT m_DrawResult = S_OK;
    GAMEINIT_STATE GameInitState = GAMEINIT_STATE_IN_PROGRESS;

    for(;;)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;

        // Updating time
        m_pTime->UpdateTime();

        // init game
        if (GameInitState == GAMEINIT_STATE_IN_PROGRESS)
            GameInitState = m_pGame->Initialize(g_hInst, g_hWnd);

        // run game
        else if (GameInitState == GAMEINIT_STATE_OK)
        {
            if (m_DrawResult == S_OK)
            {
                // Game run
                if (!m_pGame->Run(m_pTime->GetCurTime(), (UINT)m_pTime->GetTimeElapsed()))
                    break;

                // Game render
                m_DrawResult = m_pGame->Draw();
            }
            else    // if draw device is lost, reset it
            {
                Sleep(100);
                m_DrawResult = m_pGame->ResetD3DXDevice(g_hWnd);
            }
        }
        // if init game failed, end game
        else
            break;
    }
    ReleaseObjects();

    CoUninitialize();

    return msg.wParam;
}

void ReleaseObjects()
{
    // release game
    if (m_pGame)
        m_pGame->Del();

    // release Time
    if (m_pTime)
        m_pTime->Del();

    // release logfile
    if (m_pLogfile)
        m_pLogfile->Del();
}

// this is the main message handler for the program
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}

//Erstellt das Main Window
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow, CGameInfo *GameInfo )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "Engine";
    wcex.hIconSm = NULL;
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    GameInfo->GetWindowSize( (unsigned int &)rc.right, (unsigned int &)rc.bottom);
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( "Engine", (LPCSTR)GameInfo->GetProgramName().c_str(), GameInfo->IsWindowed() ? (WS_OVERLAPPEDWINDOW | WS_VISIBLE) : (WS_EX_TOPMOST | WS_POPUP),
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL );

    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    return S_OK;
}