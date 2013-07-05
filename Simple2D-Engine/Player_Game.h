#ifndef PLAYER_GAME_H
#define PLAYER_GAME_H

#include "Player.h"
#include "DynamicObject.h"

namespace PLAYER
{
    class Player_Game : public Player
    {
    public:
        Player_Game(const KEY::Keyboard &keyboard);

        void setControle(MAP::OBJECT::DynamicObjectPtr pObject);
        inline MAP::OBJECT::DynamicObjectPtr getControledObject() { return m_pControledObject; }
        void releaseControle();

    private:
        MAP::OBJECT::DynamicObjectPtr m_pControledObject;
    };
}
#endif