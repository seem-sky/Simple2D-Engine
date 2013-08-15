#include "Player.h"

using namespace PLAYER;

Player::Player()
{}

void Player::update(uint32 uiDiff)
{
    for (auto &pair : m_KeyBindings)
    {
        if (KEY::GlobalKeyboard::get()->isKeyPressed(pair.first))
            pair.second();
    }
}

void Player::addKeyBind(KEY::Keys key, VoidFunction function, bool releaseOldBind)
{
    if (releaseOldBind)
        releaseKeyBind(key);
    m_KeyBindings.insert(std::make_pair(key, function));
}

void Player::releaseKeyBind(KEY::Keys key)
{
    auto itr = m_KeyBindings.find(key);
    if (itr != m_KeyBindings.cend())
        m_KeyBindings.erase(itr);
}