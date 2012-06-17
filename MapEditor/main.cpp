#include "MainWindow.h"
#include <QT/qapplication.h>

int main(int argc, char* argv[])
{
    MainWindow t_MainWindow;
    t_MainWindow.show();
    QApplication app(argc, argv);
    return app.exec();
}