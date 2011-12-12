#include "DirectInput.h"

DirectInput::DirectInput()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "DirectInput : ";

    m_DirectInput       = NULL;
    m_DIKeyboard        = NULL;
    m_DIMouse           = NULL;
    m_v2MouseKoord.x    = 0;
    m_v2MouseKoord.y    = 0;
    m_bMouseKlick       = false;

    ClearKeyStateKeyboard();
}

DirectInput::~DirectInput()
{
    CleanUp();
}

BOOL DirectInput::Init(HINSTANCE hInstance, HWND hWnd)
{
    HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)(&m_DirectInput), NULL);
    if(hr != S_OK)
        return FALSE;

    // init keyboard
    hr = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_DIKeyboard, NULL);
    if(hr != S_OK)
        return FALSE;

    hr = m_DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
    if(hr != S_OK)
        return FALSE;

    hr = m_DIKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
    if(hr != S_OK)
        return FALSE;

    hr = m_DIKeyboard->Acquire();
    if(hr != S_OK)
        return FALSE;

    // init mouse
    hr = m_DirectInput->CreateDevice(GUID_SysMouse, &m_DIMouse, NULL);
    if(hr != S_OK)
        return FALSE;

    hr = m_DIMouse->SetDataFormat(&c_dfDIMouse);
    if(hr != S_OK)
        return FALSE;

    hr = m_DIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if(hr != S_OK)
        return FALSE;

    hr = InitMousePuffer( 20 );
    if(hr != S_OK)
        return FALSE;

    return TRUE;
}

//free direct input object
void DirectInput::CleanUp()
{
    if (m_DIKeyboard)
    {
        m_DIKeyboard->Unacquire();
        m_DIKeyboard->Release();
        m_DIKeyboard = NULL;
    }

    if (m_DIMouse)
    {
        m_DIMouse->Unacquire();
        m_DIMouse->Release();
        m_DIMouse = NULL;
    }

    if(m_DirectInput)
    {
        m_DirectInput->Release();
        m_DirectInput = NULL;
    }
}
void DirectInput::SetKeyStateKeyboard()
{
    if(FAILED( m_DIKeyboard->GetDeviceState( sizeof(m_aKeyState), &m_aKeyState)))
        m_DIKeyboard->Acquire();
}

bool DirectInput::GetKeyStateKeyboard(UINT Key)
{
    return m_aKeyState[Key];
}

void DirectInput::ClearKeyStateKeyboard()
{
    memset(&m_aKeyState, NULL, sizeof(m_aKeyState));
}

HRESULT DirectInput::InitMousePuffer(int PufferSize)
{
    ZeroMemory( &DIProperties, sizeof(DIProperties ));
    DIProperties.diph.dwSize            = sizeof(DIPROPDWORD);
    DIProperties.diph.dwHeaderSize      = sizeof(DIPROPHEADER);
    DIProperties.diph.dwObj             = 0;
    DIProperties.diph.dwHow             = DIPH_DEVICE;

    DIProperties.dwData                 = PufferSize;

    return m_DIMouse->SetProperty( DIPROP_BUFFERSIZE, &DIProperties.diph );
}

void DirectInput::ProcessInput( void )
{
    DWORD NumElements = 1;
    DIDEVICEOBJECTDATA data;
    ZeroMemory( &data, sizeof( data ) );
    if( FAILED ( m_DIMouse->GetDeviceData( sizeof( data ), &data, &NumElements, 0 ) ) )
    {
        m_DIMouse->Acquire();
        return;
    }
    //get mousemovement
    switch( data.dwOfs )
    {
    case DIMOFS_X:
        m_v2MouseKoord.x += data.dwData;        //move in X
        break;
    case DIMOFS_Y:
        m_v2MouseKoord.y += data.dwData;        //move in Y
        break;
    case DIMOFS_BUTTON1:
        break;
    }
}