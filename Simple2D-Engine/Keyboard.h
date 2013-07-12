#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Global.h"
#include "GameWindow.h"
#include "Singleton.h"
#include <bitset>

namespace KEY
{
    enum Keys
    {
        KEY_ESCAPE,
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,
        KEY_PRINT,
        Key_PAUSE,
        KEY_INSERT,
        KEY_DELETE,
        KEY_POS1,
        KEY_PAGE_UP,
        KEY_PAGE_DOWN,
        KEY_END,
        KEY_CIRCUMPLEX,

        // numbers
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_0,

        KEY_ß,
        KEY_APOSTROPH,
        KEY_RETURN,
        KEY_TAB,
        KEY_ENTER,
        KEY_CAPS_LOCK,
        KEY_SHIFT,
        KEY_SHIFT_RIGHT,
        KEY_CTRL,
        KEY_ALT,
        KEY_SPACE,
        KEY_ALT_GR,
        KEY_STRG_RIGHT,

        // direction keys
        KEY_UP,
        KEY_RIGHT,
        KEY_DOWN,
        KEY_LEFT,

        KEY_PLUS,
        KEY_DIAMOND,
        KEY_MINUS,
        KEY_POINT,
        KEY_COMMA,

        KEY_LESS,

        // letters
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,

        // numpad
        KEY_NUM,
        KEY_NUM_DIVIDE,
        KEY_NUM_MULTIPLY,
        KEY_NUM_MINUS,
        KEY_NUM_PLUS,
        KEY_NUM_ENTER,
        KEY_NUM_INSERT,
        KEY_NUM_0,
        KEY_NUM_1,
        KEY_NUM_2,
        KEY_NUM_3,
        KEY_NUM_4,
        KEY_NUM_5,
        KEY_NUM_6,
        KEY_NUM_7,
        KEY_NUM_8,
        KEY_NUM_9,

        KEY_NONE,
    };
    const uint32 KEY_COUNT = 99;

    //ToDo: Store this in separate class, in a layer above this.

    //enum KeyState
    //{
    //    STATE_DOWN,
    //    STATE_UP,
    //    STATE_JUST_PRESSED,
    //    STATE_JUST_RELEASED,
    //};

    //class Key
    //{
    //public:
    //    Key();

    //    inline KeyState getKeyState() const { return m_KeyState; }
    //    void changeKeyState(KeyState state);

    //    void update(uint32 uiDiff);

    //private:
    //    KeyState m_KeyState;
    //    uint32 m_Time;
    //};

    class Keyboard
    {
    public:
        Keyboard();

        inline void changeKeyState(Keys key, bool down) { m_KeyStates.at(key) = down; }
        inline bool isKeyPressed(Keys key) const { return m_KeyStates.at(key); }
        inline void resetKeyStates() { m_KeyStates.reset(); }

    private:
        std::bitset<KEY_COUNT> m_KeyStates;
    };
    typedef TSingleton<Keyboard> GlobalKeyboard;

    Keys wrapKeyFromQT(int key);
}
#endif