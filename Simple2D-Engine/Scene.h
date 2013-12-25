#ifndef SCENE_H
#define SCENE_H

#include <QtWidgets/QGraphicsScene>
#include <QtCore/QTime>
#include <Global.h>

namespace GAME_LOGIC
{
    class Game;

    namespace SCENE
    {
        /*#####
        # SceneView
        #####*/
        class Scene;
        class SceneView : public QGraphicsScene
        {
        protected:
            virtual void drawForeground(QPainter* painter, const QRectF& rect);

            void drawFPSCounter(QPainter* painter, const QRectF& rect);

        public:
            SceneView(Scene* pScene) : QGraphicsScene(), m_pScene(pScene)
            {}

        protected:
            Scene* m_pScene;

        private:
            QTime m_FPSCounter;
        };

        /*#####
        # Scene
        #####*/
        enum UpdateResult
        {
            NONE                    = 0x000,
            CLOSE_GAME              = 0x001,
            TOGGLE_FULL_SCREEN      = 0x002
        };

        class SceneMgr;
        class Scene
        {
        private:
            void _updateFPS(uint32 uiDiff);

        public:
            Scene(Game& game);

            SceneView* getSceneView() { return m_pSceneView; }

            virtual void update(uint32 uiDiff) = 0;

            inline bool isFPSShown() const { return m_ShowFPS; }
            inline void showFPS(bool show = true) { m_ShowFPS = show; }

        protected:
            Game& m_Game;
            SceneView* m_pSceneView;

        private:
            bool m_ShowFPS;
        };
    }
}
#endif