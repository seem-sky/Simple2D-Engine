#include <windows.h>
#include <windowsx.h>
#include "Direct3D.h"
#include "GameInfo.h"
//#include "DirectInput.h"

// globals
HINSTANCE g_hInst       = NULL;
HWND g_hWnd             = NULL;
CLogfile *m_pLogfile    = NULL;

HRESULT InitWindow ( HINSTANCE hInstance, int nCmdShow, CGameInfo GameInfo );
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

    // read game ini data
    CGameInfo GameInfo;
    if (!GameInfo.ReadFile(GAME_DATA_GAME_INI))
        GameInfo.CreateIniByDefault();
    else
        BASIC_LOG("ENGINE::MAIN : Read and interpret " + GAME_DATA_GAME_INI + ".");

    if( FAILED(InitWindow( hInstance, nCmdShow, GameInfo) ) )
    {
        ERROR_LOG("ENGINE::MAIN : Unable to register WindowClass or create window.");
        ReleaseObjects();
        return 0;
    }
    else
        BASIC_LOG("ENGINE::MAIN : Succesfully create window.");

   // enter the main loop:
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            // Run game code here
            // ...
            // ...
        }
    }
    ReleaseObjects();

    return msg.wParam;
}

void ReleaseObjects()
{
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
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow, CGameInfo GameInfo )
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
    GameInfo.GetWindowSize( (unsigned int &)rc.right, (unsigned int &)rc.bottom);
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    if (GameInfo.IsWindowed())
        g_hWnd = CreateWindow( "Engine", (LPCSTR)GameInfo.GetProgramName().c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL );
    else
        g_hWnd = CreateWindow( "Engine", (LPCSTR)GameInfo.GetProgramName().c_str(), WS_EX_TOPMOST | WS_POPUP,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    return S_OK;
}