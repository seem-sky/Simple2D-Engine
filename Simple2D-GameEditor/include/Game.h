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

        void setSceneView(SCENE::SceneView* pScene);
        void setScene(SCENE::ScenePtr pScene);

        bool isFullScreen() const;
        void toggleFullScreen();

    private:
        bool m_closeGame;
        SCENE::SceneMgr m_SceneMgr;
    };
}
#endif