#include "Player.h"

using namespace PLAYER;

Player::Player()
{}

void Player::update(uint32 uiDiff)
{
    for (auto &pair : m_KeyBindings)
    {
        if (pair.second.m_uiRepeatTimer < pair.second.m_uiRepeatTime)
            pair.second.m_uiRepeatTimer += uiDiff;
        if (KEY::GlobalKeyboard::get()->isKeyPressed(pair.first) && pair.second.m_uiRepeatTimer >= pair.second.m_uiRepeatTime)
        {
            pair.second.m_Function();
            pair.second.m_uiRepeatTimer = 0;
        }
    }
}

void Player::addKeyBind(KEY::Keys key, VoidFunction function, uint32 m_uiRepeatTime, bool releaseOldBind)
{
    if (releaseOldBind)
        releaseKeyBind(key);
    Hotkey newHotkey;
    newHotkey.m_Function = function;
    newHotkey.m_uiRepeatTime = m_uiRepeatTime;
    m_KeyBindings.insert(std::make_pair(key, newHotkey));
}

void Player::releaseKeyBind(KEY::Keys key)
{
    auto itr = m_KeyBindings.find(key);
    if (itr != m_KeyBindings.cend())
        m_KeyBindings.erase(itr);
}