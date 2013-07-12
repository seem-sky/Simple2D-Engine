#include "Program.h"
#include "Game.h"
#include "SplashScreen.h"
#include "GameScene.h"
#include <QtCore/QTime>
#include <QtGui/QIcon>
#include <windows.h>
#include <QtCore/QDebug>

using namespace PROGRAM;

CMD_Params::CMD_Params() : m_DebugMode(false)
{}

void CMD_Params::parseParam(const QString &param)
{
    if (param == "-debug")
        m_DebugMode = true;
    else
        qDebug() << param << " is an unknown command line command.";
}

void Program::setApplication(ApplicationPtr pApp)
{
    m_pApplication = std::move(pApp);
}

void Program::init(int argc, char *argv[])
{
    // parse command line commands
    for (int32 i = 1; i < argc; ++i)
        m_CMDparams.parseParam(QString(argv[i]));
    // setup application name
    m_pQtApplication = QApplicationPtr(new QApplication(argc, argv));
    m_pQtApplication->setWindowIcon(QIcon("icons/Logo-clean.png"));
    m_pQtApplication->setApplicationVersion(QString::fromStdString(ENGINE_VERSION));
    m_pQtApplication->setApplicationDisplayName(m_pQtApplication->applicationName() + " " + m_pQtApplication->applicationVersion());

    // show engine splash screen for 5sec
    if (!m_CMDparams.isDebugMode())
    {
        setApplication(PROGRAM::ApplicationPtr(new SPLASH_SCREEN::SplashScreen()));
        m_pQtApplication->processEvents();
        Sleep(5000);
    }
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