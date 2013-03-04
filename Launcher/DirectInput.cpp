#include "DirectInput.h"

DirectInput::DirectInput() : m_DirectInput(0), m_DIKeyboard(0), m_DIMouse(0), m_v2MouseCoord(0,0), m_bMouseClick(false), TSingleton()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "DirectInput : ";

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

    hr = InitMouseBuffer( 20 );
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

bool DirectInput::SetKeyStateKeyboard()
{
    if(FAILED( m_DIKeyboard->GetDeviceState( sizeof(m_aKeyState), &m_aKeyState)))
        if (FAILED(m_DIKeyboard->Acquire()))
            return false;

    return true;
}

bool DirectInput::GetKeyStateKeyboard(uint32 Key)
{
    return m_aKeyState[Key];
}

void DirectInput::ClearKeyStateKeyboard()
{
    memset(&m_aKeyState, NULL, sizeof(m_aKeyState));
}

HRESULT DirectInput::InitMouseBuffer(int BufferSize)
{
    ZeroMemory( &DIProperties, sizeof(DIProperties ));
    DIProperties.diph.dwSize            = sizeof(DIPROPDWORD);
    DIProperties.diph.dwHeaderSize      = sizeof(DIPROPHEADER);
    DIProperties.diph.dwObj             = 0;
    DIProperties.diph.dwHow             = DIPH_DEVICE;

    DIProperties.dwData                 = BufferSize;

    return m_DIMouse->SetProperty( DIPROP_BUFFERSIZE, &DIProperties.diph );
}

bool DirectInput::ProcessInput( void )
{
    DWORD NumElements = 1;
    DIDEVICEOBJECTDATA data;
    ZeroMemory( &data, sizeof( data ) );
    if( FAILED ( m_DIMouse->GetDeviceData( sizeof( data ), &data, &NumElements, 0 ) ) )
    {
        m_DIMouse->Acquire();
        return false;
    }

    //get mousemovement
    switch( data.dwOfs )
    {
    case DIMOFS_X:
        m_v2MouseCoord.x += data.dwData;        //move in X
        break;
    case DIMOFS_Y:
        m_v2MouseCoord.y += data.dwData;        //move in Y
        break;
    case DIMOFS_BUTTON1:
        break;
    }

    return true;
}

bool DirectInput::GetInput()
{
    if (!SetKeyStateKeyboard())
        return false;

    /*if (!ProcessInput())
        return false;*/

    return true;
}