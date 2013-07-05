#ifndef GAME_H
#define GAME_H

#include "Application.h"
#include "SceneMgr.h"
#include "Keyboard.h"

namespace GAME_LOGIC
{
    class Game : public PROGRAM::Application
    {
    public:
        Game();

        virtual int update(uint32 uiDiff);

        inline void closeGame() { m_closeGame = true; }

        void setSceneView(SCENE::SceneView *pScene);
        void setScene(SCENE::ScenePtr pScene);

        inline KEY::Keyboard& getKeyboard() { return m_Keyboard; }

    private:
        bool m_closeGame;
        SCENE::SceneMgr m_SceneMgr;
        KEY::Keyboard m_Keyboard;
    };
}
#endif