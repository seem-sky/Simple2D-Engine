#include "Player_Game.h"
#include "Map.h"

using namespace PLAYER;

GamePlayer::GamePlayer(MAP::MapMgr &mapMgr) : Player(), m_MapMgr(mapMgr), m_Camera(this), m_MapGUID(0), m_ObjectGUID(0)
{}

void GamePlayer::update(uint32 uiDiff)
{
    m_Camera.update(uiDiff);
    Player::update(uiDiff);
}

MAP::OBJECT::DynamicObjectPtr GamePlayer::getControledObject()
{
    MAP::OBJECT::DynamicObjectPtr pObj;
    MAP::MapPtr pMap;
    if (m_MapMgr.getItem(getMapGUID(), pMap))
        pObj = std::dynamic_pointer_cast<MAP::OBJECT::DynamicObject>(pMap->getWorldObject(m_ObjectGUID));
    return pObj;
}

MAP::ConstMapPtr GamePlayer::getMap() const
{
    MAP::ConstMapPtr pMap;
    m_MapMgr.getItem(getMapGUID(), pMap);
    return pMap;
}