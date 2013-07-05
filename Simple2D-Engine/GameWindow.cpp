#include "GameWindow.h"
#include "Game.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>

using namespace GAME_LOGIC;

GameWindow::GameWindow(Game *pGame) : QMainWindow(), m_pGame(pGame)
{
    m_Viewer.setParent(this);
    m_Viewer.setGeometry(0, 0, width(), height());
    m_Viewer.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_Viewer.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_Viewer.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_Viewer.setScene(new QGraphicsScene(&m_Viewer));
    resize(1024, 576);
    //setWindowState(Qt::WindowFullScreen);
}

void GameWindow::closeEvent(QCloseEvent *pEvent)
{
    m_pGame->closeGame();
}

void GameWindow::resizeEvent(QResizeEvent *pEvent)
{
    m_Viewer.resize(pEvent->size());
}

void GameWindow::setSceneView(SCENE::SceneView *pScene)
{
    m_Viewer.setScene(pScene);
    m_Viewer.scene()->setParent(&m_Viewer);
}