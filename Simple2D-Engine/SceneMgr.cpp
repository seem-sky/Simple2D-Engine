#include "SceneMgr.h"
#include "Game.h"

using namespace GAME_LOGIC;
using namespace SCENE;

SceneMgr::SceneMgr(Game *pGame) : Manager(), m_pGame(pGame)
{}

void SceneMgr::setNew(ScenePtr &pNew)
{
    Manager::setNew(pNew);
    m_pGame->setSceneView(getCurrent()->getSceneView());
}
