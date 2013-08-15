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

    class GameViewer : public QGraphicsView
    {
    protected:
        bool eventFilter(QObject *pObj, QEvent *pEvent);

    public:
        GameViewer(QWidget *pParent = 0);
    };

    class GameWindow : public QMainWindow
    {
    protected:
        void closeEvent(QCloseEvent *pEvent);
        void resizeEvent(QResizeEvent *pEvent);
        void keyPressEvent(QKeyEvent *pEvent);
        void keyReleaseEvent(QKeyEvent *pEvent);
        bool eventFilter(QObject *pObj, QEvent *pEvent);

    public:
        GameWindow(Game *pGame);

        void setSceneView(SCENE::SceneView *pScene);
        void setFullScreen(bool fullScreen = true);

    private:
        Game *m_pGame;
        GameViewer m_Viewer;
    };
}
#endif