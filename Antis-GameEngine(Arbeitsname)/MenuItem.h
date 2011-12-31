#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <d3dx9.h>
#include <string>

class MenuItem
{
public:
    MenuItem(void) : m_sItemName("No name set") {};
    virtual ~MenuItem(void){};

    inline std::string GetItemName() { return m_sItemName; }
    inline void SetItemName(std::string sNewName) { m_sItemName = sNewName; }

    /*#####
    ## menu item AI
    #####*/
    // if item is entered
    virtual void OnItemEnter() {};
    // if item selected
    virtual void OnItemSelect() {};
    // if item lost selection
    virtual void OnItemLostSelection() {};

private:
    std::string m_sItemName;
    D3DXCOLOR m_FontColor;
};
#endif;