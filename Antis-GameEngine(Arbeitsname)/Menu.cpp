#include "Menu.h"
#include "DirectFont.h"
#include "Game.h"

/*#####
## menu basic
#####*/
Menu::Menu(bool UpdateGame, bool DrawMap, Menu *pParentMenu, bool DrawParentMenu) :
    m_bUpdateGame(UpdateGame), m_bDrawMap(DrawMap), m_uiSelectedItem(0), m_pParentMenu(pParentMenu), m_bDrawParentMenu(DrawParentMenu)
{}

Menu::~Menu(void)
{
    // delete all menu items
    for (MenuItems::iterator itr = m_MenuItemList.begin(); itr != m_MenuItemList.end(); ++itr)
        delete *itr;
}

void Menu::ChangeSelectedItem(MenuMove move)
{
    // if selected item is not valid, reset it
    if (m_uiSelectedItem > m_MenuItemList.size())
    {
        if (m_MenuItemList.empty())
            m_uiSelectedItem = 0;
        else
            m_uiSelectedItem = 1;
    }

    // if empty, return
    if (m_MenuItemList.empty())
        return;

    // send action to item
    m_MenuItemList[m_uiSelectedItem-1]->OnItemLostSelection();

    switch(move)
    {
    case MENU_MOVE_UP:
        m_uiSelectedItem--;
        if (m_uiSelectedItem == 0)
            m_uiSelectedItem = m_MenuItemList.size();
        break;
    case MENU_MOVE_DOWN:
        m_uiSelectedItem++;
        if (m_uiSelectedItem > m_MenuItemList.size())
            m_uiSelectedItem = 1;
        break;
    default:
        break;
    }

    // send action to item
    m_MenuItemList[m_uiSelectedItem-1]->OnItemSelect();
}

void Menu::DrawMenu()
{
    // if has parent menu
    if (m_pParentMenu && m_bDrawParentMenu)
        m_pParentMenu->DrawMenu();

    // if item list empty, return
    if (m_MenuItemList.empty())
        return;

    // iterate through items
    RECT rect = { 0, 0, 640, 480 };
    for (uint32 i = 0; i < m_MenuItemList.size(); i++)
    {
        DirectFont::DrawFont(m_MenuItemList[i]->GetItemName(), rect, D3DXCOLOR(1, 0.25, 0.75, 1), i+1 == m_uiSelectedItem ? 32 : 24, 500, "Comic Sans MS");
        rect.top += 50;
    }
}

void Menu::OnMenuUp()
{
    ChangeSelectedItem(MENU_MOVE_UP);
}

void Menu::OnMenuDown()
{
    ChangeSelectedItem(MENU_MOVE_DOWN);
}

void Menu::OnMenuLeft()
{
    ChangeSelectedItem(MENU_MOVE_UP);
}

void Menu::OnMenuRight()
{
    ChangeSelectedItem(MENU_MOVE_DOWN);
}

void Menu::OnMenuEnter()
{
    if (!m_uiSelectedItem || m_uiSelectedItem > m_MenuItemList.size())
        return;

    m_MenuItemList[m_uiSelectedItem-1]->OnItemEnter();
}

void Menu::OnMenuOpen()
{
    if (m_MenuItemList.empty())
        return;

    m_uiSelectedItem = 1;
}

void Menu::CloseMenu()
{
    OnMenuClose();

    delete this;
}

void Menu::CloseAllMenus()
{
    if (Menu *pMenu = GetParentMenu())
        pMenu->CloseAllMenus();

    delete this;
}

/*#####
## main menu
#####*/
MenuMainMenu::MenuMainMenu(bool UpdateGame, bool DrawMap, Menu *pParentMenu, bool DrawParentMenu) : Menu(UpdateGame, DrawMap, pParentMenu, DrawParentMenu)
{
    m_MenuItemList.push_back(new MenuItemCloseGame());
    m_MenuItemList.push_back(new MenuItemCloseMenu());
}

/*#####
## menu close game
#####*/
MenuCloseGame::MenuCloseGame(bool UpdateGame, bool DrawMap, Menu *pParentMenu, bool DrawParentMenu) : Menu(UpdateGame, DrawMap, pParentMenu, DrawParentMenu)
{
    MenuItem *pItem = new MenuItemCloseMenu();
    pItem->SetItemName("Nein");
    m_MenuItemList.push_back(pItem);
    m_MenuItemList.push_back(new MenuItemCloseGameConfirm());
}