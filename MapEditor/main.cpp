#include "MainWindow.h"
#include <QTGui/QApplication>
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    QApplication t_app(nCmdShow, (char**)lpCmdLine);
    MainWindow t_MainWindow;
    t_MainWindow.show();
    return t_app.exec();
}