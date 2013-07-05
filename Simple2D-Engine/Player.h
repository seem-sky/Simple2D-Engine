#ifndef PLAYER_H
#define PLAYER_H

#include "Global.h"
#include "Keyboard.h"
#include <unordered_map>
#include <functional>

namespace PLAYER
{
    typedef std::function<void()> VoidFunction;
    typedef std::unordered_map<KEY::Keys, VoidFunction> KeyFunctionUnorderedMap;

    class Player
    {
    public:
        Player(const KEY::Keyboard &keyboard);
        virtual void update(uint32 uiDiff);

        void addKeyBind(KEY::Keys key, VoidFunction function, bool releaseOldBind);
        void releaseKeyBind(KEY::Keys key);

    private:
        KeyFunctionUnorderedMap m_KeyBindings;
        const KEY::Keyboard &m_Keyboard;
    };
    typedef std::unique_ptr<Player> PlayerPtr;
}
#endif