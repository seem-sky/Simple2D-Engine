#include <windows.h>
#include "GameLoop.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow)
{
    GameLoop gameLoop;
    gameLoop.exec();
}