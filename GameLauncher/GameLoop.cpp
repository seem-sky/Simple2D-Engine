#include "GameLoop.h"
#include <QTGui/QApplication>

GameLoop::GameLoop() : pMainWindow(new QMainWindow())
{
    pMainWindow->show();
}

int GameLoop::exec()
{
    QApplication::processEvents();
    return 0;
}