#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsView>

namespace GAME_LOGIC
{
    namespace SCENE
    {
        class SceneView;
    }
    class Game;

    class GameWindow : public QMainWindow
    {
    protected:
        void closeEvent(QCloseEvent *pEvent);
        void resizeEvent(QResizeEvent *pEvent);
        void keyPressEvent(QKeyEvent *pEvent);
        void keyReleaseEvent(QKeyEvent *pEvent);
        bool event(QEvent *pEvent);

    public:
        GameWindow(Game *pGame);

        void setSceneView(SCENE::SceneView *pScene);

    private:
        Game *m_pGame;
        QGraphicsView m_Viewer;
    };
}
#endif