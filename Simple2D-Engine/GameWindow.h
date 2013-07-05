#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsView>
#include "Scene.h"

namespace GAME_LOGIC
{
    class Game;

    class GameWindow : public QMainWindow
    {
    protected:
        void closeEvent(QCloseEvent *pEvent);
        void resizeEvent(QResizeEvent *pEvent);

    public:
        GameWindow(Game *pGame);

        void setSceneView(SCENE::SceneView *pScene);

    private:
        Game *m_pGame;
        QGraphicsView m_Viewer;
    };
}
#endif