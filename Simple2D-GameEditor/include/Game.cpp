#include "Game.h"
#include "GameWindow.h"
#include "GameScene.h"

using namespace GAME_LOGIC;

Game::Game() : Application(), m_closeGame(false), m_SceneMgr(this)
{
    m_pWidget = QWidgetPtr(new GameWindow(this));
    m_pWidget->show();
    setScene(SCENE::ScenePtr(new SCENE::GameScene(m_SceneMgr, m_Keyboard)));
}

int Game::update(uint32 uiDiff)
{
    // update game logic
    m_SceneMgr.updateCurrent(uiDiff);
    return m_closeGame;
}

void Game::setSceneView(SCENE::SceneView *pScene)
{
    if (GameWindow *pWindow = dynamic_cast<GameWindow*>(&*m_pWidget))
        pWindow->setSceneView(pScene);
}

void Game::setScene(SCENE::ScenePtr pScene)
{
    m_SceneMgr.change(pScene);
}