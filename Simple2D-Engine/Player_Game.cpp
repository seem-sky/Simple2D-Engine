#include "Player_Game.h"
#include "Map.h"

using namespace PLAYER;

GamePlayer::GamePlayer(MAP::MapMgr& mapMgr) : Player(), m_MapMgr(mapMgr), m_Camera(this), m_MapGUID(0), m_ObjectGUID(0)
{}

void GamePlayer::update(uint32 uiDiff)
{
    m_Camera.update(uiDiff);
    Player::update(uiDiff);
}

const MAP::OBJECT::WorldObject* GamePlayer::getControledObject()
{
    //if (auto pMap = m_MapMgr.getItem(getMapGUID(), pMap))
    //    pObj = std::dynamic_pointer_cast<MAP::OBJECT::DynamicObject>(pMap->getWorldObject(m_ObjectGUID));
    return nullptr;
}