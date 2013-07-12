#ifndef SCENE_H
#define SCENE_H

#include <QtWidgets/QGraphicsScene>
#include <QtCore/QTime>
#include "Global.h"
#include "Player.h"

namespace GAME_LOGIC
{
    namespace SCENE
    {
        /*#####
        # SceneView
        #####*/
        class Scene;
        class SceneView : public QGraphicsScene
        {
        protected:
            virtual void drawForeground(QPainter *painter, const QRectF &rect);

            void drawFPSCounter(QPainter *painter, const QRectF &rect);

        public:
            SceneView(Scene *pScene) : QGraphicsScene(), m_pScene(pScene)
            {}

        protected:
            Scene *m_pScene;

        private:
            QTime m_FPSCounter;
        };

        /*#####
        # Scene
        #####*/
        class SceneMgr;
        class Scene
        {
        private:
            void _updateFPS(uint32 uiDiff);

        public:
            Scene(SceneMgr &sceneMgr);

            SceneView* getSceneView() { return m_pSceneView; }

            virtual void update(uint32 uiDiff) = 0;

            inline bool isFPSShown() const { return m_ShowFPS; }
            inline void showFPS(bool show = true) { m_ShowFPS = show; }

        protected:
            SceneMgr &m_SceneMgr;
            SceneView *m_pSceneView;
            PLAYER::PlayerPtr m_pPlayer;

        private:
            bool m_ShowFPS;
        };
    }
}
#endif