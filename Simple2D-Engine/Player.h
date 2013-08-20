#ifndef PLAYER_H
#define PLAYER_H

#include "Keyboard.h"
#include <unordered_map>
#include <functional>

namespace PLAYER
{
    typedef std::function<void()> VoidFunction;
    struct Hotkey
    {
        Hotkey() : m_uiRepeatTime(250), m_uiRepeatTimer(0)
        {}

        uint32 m_uiRepeatTime;
        uint32 m_uiRepeatTimer;
        VoidFunction m_Function;
    };
    typedef std::unordered_map<KEY::Keys, Hotkey> HotkeyUMap;

    class Player
    {
    public:
        Player();
        virtual void update(uint32 uiDiff);

        void addKeyBind(KEY::Keys key, VoidFunction function, uint32 m_uiRepeatTime = 250, bool releaseOldBind = true);
        void releaseKeyBind(KEY::Keys key);

    private:
        HotkeyUMap m_KeyBindings;
    };
    typedef std::unique_ptr<Player> PlayerPtr;
}
#endif