#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QtGui/QIcon>

#ifndef DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("icons/Logo-clean.png"));
    app.setApplicationVersion(QString::fromStdString(ENGINE_VERSION));
    app.setApplicationDisplayName(app.applicationName() + " " + app.applicationVersion());
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}