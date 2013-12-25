#ifndef SCENE_MGR_H
#define SCENE_MGR_H

#include "Scene.h"
#include "Manager.h"

namespace GAME_LOGIC
{
    class Game;
    namespace SCENE
    {
        typedef std::unique_ptr<Scene> ScenePtr;
        class SceneMgr : public Manager<Scene>
        {
        private:
            void setNew(ScenePtr& pNew);

        public:
            SceneMgr(Game* pGame);

        private:
            Game* m_pGame;
        };
    }
}
#endif