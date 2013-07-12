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
        Player();
        virtual void update(uint32 uiDiff);

        void addKeyBind(KEY::Keys key, VoidFunction function, bool releaseOldBind = false);
        void releaseKeyBind(KEY::Keys key);

    private:
        KeyFunctionUnorderedMap m_KeyBindings;
    };
    typedef std::unique_ptr<Player> PlayerPtr;
}
#endif