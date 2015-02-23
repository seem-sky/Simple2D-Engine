#include "WorldObject.h"

using namespace MAP;
using namespace OBJECT;
using namespace DATABASE::PROTOTYPE;

WorldObject::WorldObject(const DATABASE::DatabaseMgr& DBMgr, uint32 GUID, uint32 ID) : Object(GUID), m_Direction(MAP::MAP_DATA::MapDirection::down), m_DBMgr(DBMgr)
{
}

void WorldObject::_setupFromPrototype(const DATABASE::PROTOTYPE::WORLD_OBJECT::WorldObjectPrototype* pWorldObject)
{
    if (!pWorldObject)
        assert("No empty prototypes allowed!");

    // get animations
    for (uint32 i = 0; i < pWorldObject->getAnimationCount(); ++i)
    {
        auto& info = pWorldObject->getAnimationModule().getAnimationInfo(i);
        m_Animations.insert(std::make_pair(info.m_AnimationTypeID, info.m_ID));
    }
    _setCurrentAnimation(static_cast<uint32>(m_Direction));

    //other infos
    m_BoundingRect = pWorldObject->getBoundingRect();
}

void WorldObject::_setCurrentAnimation(uint32 pose)
{
    auto itr = m_Animations.find(pose);
    if (itr == m_Animations.cend())
        return;
    if (auto pAnimation = m_DBMgr.getAnimationDatabase()->getOriginalPrototype(itr->second))
        m_AnimationHolder.changeAnimation(pAnimation);
}

void WorldObject::update(uint32 uiDiff)
{
    // update animation
    m_AnimationHolder.update(uiDiff);
}
