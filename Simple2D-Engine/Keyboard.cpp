#include "Keyboard.h"
#include <QtWidgets/QApplication>

using namespace KEY;

const uint32 JUST_PRESSED_TIME = 100;
const uint32 JUST_RELEASED_TIME = 100;

/*#####
# Key
#####*/
//Key::Key() : m_KeyState(STATE_UP), m_Time(0)
//{}
//
//void Key::update(uint32 uiDiff)
//{
//    if (!m_Time)
//        return;
//    if (m_Time < uiDiff)
//    {
//        m_Time = 0;
//        switch (m_KeyState)
//        {
//        case STATE_JUST_PRESSED: m_KeyState = STATE_DOWN; break;
//        case STATE_JUST_RELEASED: m_KeyState = STATE_UP; break;
//        }
//    }
//    else
//        m_Time -= uiDiff;
//}
//
//void Key::changeKeyState(KeyState state)
//{
//    if (m_KeyState == state)
//        return;
//    m_KeyState = state;
//    switch (m_KeyState)
//    {
//    case STATE_JUST_PRESSED: m_Time = JUST_PRESSED_TIME; break;
//    case STATE_JUST_RELEASED: m_Time = JUST_RELEASED_TIME; break;
//    }
//}

/*#####
# Keyboard
#####*/
Keyboard::Keyboard()
{}

/*#####
# non-member functions
#####*/
Keys KEY::wrapKeyFromQT(int key)
{
    switch (key)
    {
    case Qt::Key_Escape: return KEY_ESCAPE;
    case Qt::Key_Tab: return KEY_TAB;
    case Qt::Key_Return: return KEY_RETURN;
    case Qt::Key_Enter: return KEY_ENTER;
    case Qt::Key_Insert: return KEY_INSERT;
    case Qt::Key_Delete: return KEY_DELETE;
    case Qt::Key_Pause: return Key_PAUSE;
    case Qt::Key_Print: return KEY_PRINT;
    case Qt::Key_End: return KEY_END;
    case Qt::Key_Left: return KEY_LEFT;
    case Qt::Key_Up: return KEY_UP;
    case Qt::Key_Right: return KEY_RIGHT;
    case Qt::Key_Down: return KEY_DOWN;
    case Qt::Key_PageUp: return KEY_PAGE_UP;
    case Qt::Key_PageDown: return KEY_PAGE_DOWN;
    case Qt::Key_Shift: return KEY_SHIFT;
    case Qt::Key_Alt: return KEY_ALT;
    case Qt::Key_AltGr: return KEY_ALT_GR;
    case Qt::Key_CapsLock: return KEY_CAPS_LOCK;
    case Qt::Key_NumLock: return KEY_NUM;
    case Qt::Key_F1: return KEY_F1;
    case Qt::Key_F2: return KEY_F2;
    case Qt::Key_F3: return KEY_F3;
    case Qt::Key_F4: return KEY_F4;
    case Qt::Key_F5: return KEY_F5;
    case Qt::Key_F6: return KEY_F6;
    case Qt::Key_F7: return KEY_F7;
    case Qt::Key_F8: return KEY_F8;
    case Qt::Key_F9: return KEY_F9;
    case Qt::Key_F10: return KEY_F10;
    case Qt::Key_F11: return KEY_F11;
    case Qt::Key_F12: return KEY_F12;
    case Qt::Key_Space: return KEY_SPACE;
    case Qt::Key_Apostrophe: return KEY_APOSTROPH;
    case Qt::Key_Plus: return KEY_PLUS;
    case Qt::Key_Comma: return KEY_COMMA;
    case Qt::Key_Minus: return KEY_MINUS;
    case Qt::Key_Slash: return KEY_DIAMOND;
    case Qt::Key_0: return KEY_0;
    case Qt::Key_1: return KEY_1;
    case Qt::Key_2: return KEY_2;
    case Qt::Key_3: return KEY_3;
    case Qt::Key_4: return KEY_4;
    case Qt::Key_5: return KEY_5;
    case Qt::Key_6: return KEY_6;
    case Qt::Key_7: return KEY_7;
    case Qt::Key_8: return KEY_8;
    case Qt::Key_9: return KEY_9;
    case Qt::Key_Less: return KEY_LESS;
    case Qt::Key_Question: return KEY_ß;
    case Qt::Key_A: return KEY_A;
    case Qt::Key_B: return KEY_B;
    case Qt::Key_C: return KEY_C;
    case Qt::Key_D: return KEY_D;
    case Qt::Key_E: return KEY_E;
    case Qt::Key_F: return KEY_F;
    case Qt::Key_G: return KEY_G;
    case Qt::Key_H: return KEY_H;
    case Qt::Key_I: return KEY_I;
    case Qt::Key_J: return KEY_J;
    case Qt::Key_K: return KEY_K;
    case Qt::Key_L: return KEY_L;
    case Qt::Key_M: return KEY_M;
    case Qt::Key_N: return KEY_N;
    case Qt::Key_O: return KEY_O;
    case Qt::Key_P: return KEY_P;
    case Qt::Key_Q: return KEY_Q;
    case Qt::Key_R: return KEY_R;
    case Qt::Key_S: return KEY_S;
    case Qt::Key_T: return KEY_T;
    case Qt::Key_U: return KEY_U;
    case Qt::Key_V: return KEY_V;
    case Qt::Key_W: return KEY_W;
    case Qt::Key_X: return KEY_X;
    case Qt::Key_Y: return KEY_Y;
    case Qt::Key_Z: return KEY_Z;
    }
    return KEY_NONE;
}