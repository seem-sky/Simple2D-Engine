#include "Program.h"
#include "Game.h"
#include "SplashScreen.h"
#include "GameScene.h"
#include <QtCore/QTime>
#include <QtGui/QIcon>
#include <windows.h>

using namespace PROGRAM;

void Program::setApplication(ApplicationPtr pApp)
{
    m_pApplication = std::move(pApp);
}

void Program::init(int argc, char *argv[])
{
    m_pQtApplication = QApplicationPtr(new QApplication(argc, argv));
    m_pQtApplication->setWindowIcon(QIcon("icons/Logo-clean.png"));
    m_pQtApplication->setApplicationVersion(QString::fromStdString(ENGINE_VERSION));
    m_pQtApplication->setApplicationDisplayName(m_pQtApplication->applicationName() + " " + m_pQtApplication->applicationVersion());

#ifndef DEBUG
    // show engine splash screen and hold 5sec
    setApplication(PROGRAM::ApplicationPtr(new SPLASH_SCREEN::SplashScreen()));
    m_pQtApplication->processEvents();
    Sleep(5000);
#endif
    // set "game" as new application
    setApplication(PROGRAM::ApplicationPtr(new GAME_LOGIC::Game()));
}

int Program::exec()
{
    QTime time;
    time.start();
    uint32 timeElapsed = 0;
    do
    {
        m_pQtApplication->processEvents();

        // get elapsed time since last frame
        timeElapsed = time.elapsed();
        time.restart();
    } while (!m_pApplication->update(timeElapsed));
    return 0;
}