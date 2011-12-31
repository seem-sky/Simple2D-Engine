#ifndef MENU_H
#define MENU_H

#include "DefaultMenuItems.h"
#include "vector"

typedef std::vector<MenuItem*> MenuItems;

enum MenuMove
{
    MENU_MOVE_UP,
    MENU_MOVE_DOWN,
};

/*#####
## menu AIs
#####
 - Menu basic
 - Menu close Game
*/

class Menu
{
public:
    Menu(bool UpdateGame = false, bool DrawMap = false, Menu *pParentMenu = NULL, bool DrawParentMenu = false);
    virtual ~Menu(void);

    // should update AI´s, map and all other not player related stuff?
    inline bool ShouldUpdating() { return m_bUpdateGame; }
    inline bool ShouldDrawMap() { return m_bDrawMap; }

    virtual void DrawMenu();
    virtual void UpdateMenu(const ULONGLONG CurTime, const UINT CurElapsedTime) {};

    void CloseMenu();
    void CloseAllMenus();

    inline Menu* GetParentMenu() { return m_pParentMenu; }

    /*#####
    ## menu AI
    #####*/
    // direktion keys
    virtual void OnMenuLeft();
    virtual void OnMenuRight();
    virtual void OnMenuUp();
    virtual void OnMenuDown();

    // open menu
    virtual void OnMenuOpen();
    // close Menu
    virtual void OnMenuClose() {};
    // chose an menu item
    virtual void OnMenuEnter();

protected:
    MenuItems m_MenuItemList;

private:
    bool m_bUpdateGame;
    bool m_bDrawMap;

    // stored menu items
    UINT m_uiSelectedItem;

    // parent menu
    Menu *m_pParentMenu;
    bool m_bDrawParentMenu;

    void ChangeSelectedItem(MenuMove move);
};

/* Main menu */
class MenuMainMenu : public Menu
{
public:
    MenuMainMenu(bool UpdateGame = false, bool DrawMap = false, Menu *pParentMenu = NULL, bool DrawParentMenu = false);

    // AI changes
    void OnMenuRight() {};
    void OnMenuLeft() {};
};

/* Close Game Menu */
class MenuCloseGame : public Menu
{
public:
    MenuCloseGame(bool UpdateGame = false, bool DrawMap = false, Menu *pParentMenu = NULL, bool DrawParentMenu = false);

    // AI changes
    void OnMenuUp() {};
    void OnMenuDown() {};
};
#endif;