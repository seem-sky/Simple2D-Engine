#include "DefaultMenuItems.h"
#include "Menu.h"
#include "Game.h"

void MenuItemCloseGame::OnItemEnter()
{
    if (CGame *pGame = CGame::Get())
        pGame->DisplayMenu(new MenuCloseGame(false, true, pGame->GetShownMenu()));
}

void MenuItemCloseGameConfirm::OnItemEnter()
{
    if (CGame *pGame = CGame::Get())
        pGame->CloseGame();
}

void MenuItemCloseMenu::OnItemEnter()
{
    if (CGame *pGame = CGame::Get())
        pGame->ShutDownMenu();
}