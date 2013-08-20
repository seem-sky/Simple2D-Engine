#include "GameWindow.h"
#include "Game.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>
#include "Keyboard.h"
#include "Scene.h"
//#include <QtOpenGL/QGLWidget>

using namespace GAME_LOGIC;

GameWindow::GameWindow(Game *pGame) : QMainWindow(), m_pGame(pGame), m_Viewer(this)
{
    installEventFilter(this);
    grabKeyboard();
    setStyleSheet("background-color: black;");
    //resize(640, 360);
    resize(853, 480);
    //resize(640, 480);
    setFixedSize(size());
    m_Viewer.setRenderHint(QPainter::TextAntialiasing, false);
    //setFullScreen();
}

void GameWindow::setFullScreen(bool fullScreen)
{
    if (fullScreen)
    {
        QSize oldSize = size();
        showFullScreen();
        QSize newSize = size();
        float modifier = qMin(static_cast<float>(newSize.width()) / oldSize.width(), static_cast<float>(newSize.height()) / oldSize.height());
        m_Viewer.setGeometry((newSize.width()-oldSize.width()*modifier)/2, (newSize.height()-oldSize.height()*modifier)/2,
            oldSize.width()*modifier, oldSize.height()*modifier);
        QPointF pos = m_Viewer.mapToScene(0,0);
        //m_Viewer.fitInView(pos.x(), pos.y(), oldSize.width(), oldSize.height(), Qt::KeepAspectRatioByExpanding);
        m_Viewer.scale(modifier, modifier);
    }
    else
    {
        showNormal();
        m_Viewer.move(-m_Viewer.x(), -m_Viewer.y());
        m_Viewer.resetTransform();
    }
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

void GameWindow::keyPressEvent(QKeyEvent *pEvent)
{
    //QMainWindow::keyPressEvent(pEvent);
    KEY::GlobalKeyboard::get()->changeKeyState(KEY::wrapKeyFromQT(pEvent->key()), true);
}

void GameWindow::keyReleaseEvent(QKeyEvent *pEvent)
{
    //QMainWindow::keyReleaseEvent(pEvent);
    KEY::GlobalKeyboard::get()->changeKeyState(KEY::wrapKeyFromQT(pEvent->key()), false);
}

bool GameWindow::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (pEvent->type() == QEvent::WindowDeactivate)
    {
        KEY::GlobalKeyboard::get()->resetKeyStates();
        return true;
    }
    return QMainWindow::event(pEvent);
}

GameViewer::GameViewer(QWidget *pParent) : QGraphicsView(pParent)
{
    //setupViewport(new QGLWidget(this));
    //setGeometry(0, 0, width(), height());
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewport()->installEventFilter(this);
    setLineWidth(0);
    setFrameShape(Shape::NoFrame);
}

bool GameViewer::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if(pObj == viewport() && pEvent->type() == QEvent::Wheel)
        return true;
    return QWidget::eventFilter(pObj, pEvent);
}