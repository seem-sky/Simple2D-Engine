#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "Global.h"
#include <QtWidgets/QMainWindow>
#include <boost/smart_ptr.hpp>

typedef boost::shared_ptr<QMainWindow> QMainWindowPtr;

class GameLoop
{
public:
    GameLoop();
    int exec();

private:
    QMainWindowPtr pMainWindow;
};
#endif