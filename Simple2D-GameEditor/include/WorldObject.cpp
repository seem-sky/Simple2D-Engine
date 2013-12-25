#include "WorldObject.h"

using namespace MAP;
using namespace OBJECT;

WorldObject::WorldObject(const DATABASE::DatabaseMgr& DBMgr, uint32 GUID, uint32 ID) : Object(GUID), m_Direction(DATABASE::MAP_STRUCTURE::DIRECTION_DOWN), m_DBMgr(DBMgr)
{
}

void WorldObject::_setupFromPrototype(const DATABASE::MAP_OBJECT::WorldObjectPrototype* pWorldObject)
{
    if (!pWorldObject)
        assert("No empty prototypes allowed!");

    // get animations
    for (uint32 i = 0; i < pWorldObject->getAnimationCount(); ++i)
    {
        DATABASE::MAP_OBJECT::AnimationInfo info = pWorldObject->getAnimationInfo(i);
        m_Animations.insert(std::make_pair(info.m_uiAnimationTypeID, info.m_uiAnimationID));
    }
    _setCurrentAnimation(m_Direction);

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
