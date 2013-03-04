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

    bool GetKeyStateKeyboard(uint32 Key);
    void ClearKeyStateKeyboard();

    bool GetInput();

private:
    void CleanUp();
    HRESULT InitMouseBuffer( int32 BufferSize );
    //Direct Input Object
    LPDIRECTINPUT8 m_DirectInput;

    //Devices
    //
    //keyboard
    LPDIRECTINPUTDEVICE8 m_DIKeyboard;
    //mouse
    LPDIRECTINPUTDEVICE8 m_DIMouse;

    // key state
    bool m_aKeyState[256];
    // mousebuffer
    DIPROPDWORD DIProperties;
    //mouse coord
    D3DXVECTOR2 m_v2MouseCoord;
    D3DXVECTOR2 m_v2MouseClick;
    bool m_bMouseClick;

    // look up key states
    bool SetKeyStateKeyboard();
    // look up mouse
    bool ProcessInput( void );
};
#endif