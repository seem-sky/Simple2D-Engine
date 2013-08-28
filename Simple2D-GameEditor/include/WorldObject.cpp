#include "WorldObject.h"

using namespace MAP;
using namespace OBJECT;

WorldObject::WorldObject(DATABASE::ConstDatabaseMgrPtr pDBMgr, uint32 GUID, DATABASE::ConstWorldObjectPrototypePtr pWorldObject) :
    Object(GUID), m_Direction(DATABASE::MAP_STRUCTURE::DIRECTION_DOWN), m_pDBMgr(pDBMgr)
{
    _setupFromPrototype(pWorldObject);
}

void WorldObject::_setupFromPrototype(DATABASE::ConstWorldObjectPrototypePtr pWorldObject)
{
    if (!pWorldObject)
        assert("No empty prototypes allowed!");

    // get animations
    for (uint32 i = 0; i < pWorldObject->getAnimationCount(); ++i)
    {
        DATABASE::MAP_OBJECT::AnimationInfo info = pWorldObject->getAnimationInfo(i);
        m_Animations.insert(std::make_pair(info.m_uiObjectAnimationTypeID, info.m_uiAnimationID));
    }
    _setCurrentAnimation(m_Direction);

    //other infos
    m_BoundingRect = pWorldObject->getBoundingRect();
}

void WorldObject::_setCurrentAnimation(uint32 pose)
{
    if (!m_pDBMgr)
        return;

    auto itr = m_Animations.find(pose);
    if (itr == m_Animations.cend())
        return;
    DATABASE::ConstAnimationPrototypePtr pAnimation;
    if (m_pDBMgr->getAnimationDatabase()->getItem(itr->second, pAnimation))
        m_AnimationHolder.changeAnimation(pAnimation);
}

void WorldObject::update(uint32 uiDiff)
{
    // update animation
    m_AnimationHolder.update(uiDiff);
}
