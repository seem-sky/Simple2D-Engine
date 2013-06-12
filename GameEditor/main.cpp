#include "MainWindow.h"
#include <Global.h>
#include <QTGui/QApplication>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow)
{
    QApplication app(nCmdShow, (char**)lpCmdLine);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}