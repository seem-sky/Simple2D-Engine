#include "Program.h"
#include "Game.h"

#ifndef DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
int main(int argc, char *argv[])
{
    PROGRAM::Simple2D_Engine_Program.init(argc, argv);
    return PROGRAM::Simple2D_Engine_Program.exec();
}