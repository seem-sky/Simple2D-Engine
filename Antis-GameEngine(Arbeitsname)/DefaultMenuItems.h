#ifndef DEFAULT_MENU_ITEM_H
#define DEFAULT_MENU_ITEM_H

#include "MenuItem.h"

/*#####
## menu item AIs
#####
 - MenuItemCloseGame (Open close game menu)
 - MenuItemCloseGameConfirm (Close Game)
 - MenuItemCloseMenu (Close Menu)
*/

/* Open close game menu */
struct MenuItemCloseGame : public MenuItem
{
    MenuItemCloseGame(void) : MenuItem()
    {
        SetItemName("Spiel Beenden");
    }

    void OnItemEnter();
};

/* Close Game */
struct MenuItemCloseGameConfirm : public MenuItem
{
    MenuItemCloseGameConfirm(void) : MenuItem()
    {
        SetItemName("Ja");
    }

    void OnItemEnter();
};

/*Close Menu*/
struct MenuItemCloseMenu : public MenuItem
{
    MenuItemCloseMenu() : MenuItem()
    {
        SetItemName("Zurück");
    }

    void OnItemEnter();
};
#endif;