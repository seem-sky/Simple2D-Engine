#ifndef DIRECTINPUT_H
#define DIRECTINPUT_H

//You MUST define this PRIOR to including dinput.h
#define DIRECTINPUT_VERSION 0x0800
#include <d3dx9math.h>
#include <dinput.h>
#include "Singleton.h"

class DirectInput : public TSingleton<DirectInput>
{
public:
    DirectInput();
    ~DirectInput();

    BOOL Init(HINSTANCE hInstance, HWND hWnd);

    bool GetKeyStateKeyboard(UINT Key);
    void ClearKeyStateKeyboard();

    bool GetInput();

private:
    void CleanUp();
    HRESULT InitMousePuffer( int PufferSize );
    //Direct Input Object
    LPDIRECTINPUT8 m_DirectInput;

    //Devices
    //
    //Tastatur
    LPDIRECTINPUTDEVICE8 m_DIKeyboard;
    //Maus
    LPDIRECTINPUTDEVICE8 m_DIMouse;

    // key state
    bool m_aKeyState[256];
    // mouspuffer
    DIPROPDWORD DIProperties;
    // maus koord
    D3DXVECTOR2 m_v2MouseKoord;
    D3DXVECTOR2 m_v2MouseKlick;
    bool m_bMouseKlick;

    // look up key states
    bool SetKeyStateKeyboard();
    // look up mouse
    bool ProcessInput( void );
};
#endif