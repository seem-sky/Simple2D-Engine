#include "DirectInput.h"

CDirectInput::CDirectInput()
{
    m_DirectInput       = NULL;
    m_DIKeyboard        = NULL;
    m_DIMouse           = NULL;
    m_v2MouseKoord.x    = 0;
    m_v2MouseKoord.y    = 0;
    m_bMouseKlick       = false;

    ClearKeyStateKeyboard();
}

CDirectInput::~CDirectInput()
{
    CleanUp();
}

//Erzeugen des DirectInput Objects
//returns TRUE wenn erfolgreich und FALSE wenn fehlgeschlagen
BOOL CDirectInput::Init(HINSTANCE hInstance, HWND hWnd)
{
    DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<LPVOID*>(&m_DirectInput), NULL);

    if(!m_DirectInput)
        return FALSE;
    // init keyboard
    m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_DIKeyboard, NULL);
    m_DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
    m_DIKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
    m_DIKeyboard->Acquire();
    // init mouse
    m_DirectInput->CreateDevice(GUID_SysMouse, &m_DIMouse, NULL);
    m_DIMouse->SetDataFormat(&c_dfDIMouse);
    m_DIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    InitMousePuffer( 20 );
    return TRUE;
}
//free direct input object
void CDirectInput::CleanUp()
{
    if(NULL != m_DirectInput)
    {
        m_DirectInput->Release();
        m_DirectInput = NULL;
    }
}
void CDirectInput::SetKeyStateKeyboard()
{
    if(FAILED( m_DIKeyboard->GetDeviceState( sizeof(m_aKeyState), &m_aKeyState)))
        m_DIKeyboard->Acquire();
}

bool CDirectInput::GetKeyStateKeyboard(int Key)
{
    return m_aKeyState[Key];
}

void CDirectInput::ClearKeyStateKeyboard()
{
    for(int i = 0; i < sizeof(m_aKeyState); i++)
        m_aKeyState[i] = false;
}

void CDirectInput::InitMousePuffer(int PufferSize)
{
    ZeroMemory( &DIProperties, sizeof(DIProperties ));
    DIProperties.diph.dwSize            = sizeof(DIPROPDWORD);
    DIProperties.diph.dwHeaderSize      = sizeof(DIPROPHEADER);
    DIProperties.diph.dwObj             = 0;
    DIProperties.diph.dwHow             = DIPH_DEVICE;

    DIProperties.dwData                 = PufferSize;

    m_DIMouse->SetProperty( DIPROP_BUFFERSIZE, &DIProperties.diph );
}

void CDirectInput::ProcessInput( void )
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