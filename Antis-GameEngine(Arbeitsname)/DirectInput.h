#ifndef DIRECTINPUT_H
#define DIRECTINPUT_H

//You MUST define this PRIOR to including dinput.h
#define DIRECTINPUT_VERSION 0x0800
#include <d3dx9math.h>
#include <dinput.h>
#include "Singleton.h"

class CDirectInput : public TSingleton<CDirectInput>
{
public:
    CDirectInput();
    ~CDirectInput();
    //Prototypen
    //
    BOOL Init(HINSTANCE hInstance, HWND hWnd);
    void SetKeyStateKeyboard();
    bool GetKeyStateKeyboard(int Key);
    void ClearKeyStateKeyboard();
    void ProcessInput( void );

protected:
    //Prototypen
    //
    void CleanUp();
    void InitMousePuffer( int PufferSize );
    //Direct Input Object
    LPDIRECTINPUT8 m_DirectInput;
    //Devices
    //
    //Tastatur
    LPDIRECTINPUTDEVICE8 m_DIKeyboard;
    //Maus
    LPDIRECTINPUTDEVICE8 m_DIMouse;

    //Variabeln
    //
    //Tastenstatus
    bool m_aKeyState[256];
    //MausPuffer
    DIPROPDWORD DIProperties;
    //Maus Koordinaten
    D3DXVECTOR2 m_v2MouseKoord;
    D3DXVECTOR2 m_v2MouseKlick;
    bool m_bMouseKlick;
};
#endif