#include "TransformationHolder.h"
#include "Object.h"

using namespace ENTITY;
using namespace TRANSFORMATION;

/*#####
# Transformation
#####*/
Transformation::Transformation(uint32 uiTime, Object *pOwner) : m_uiTimer(uiTime), m_pOwner(pOwner)
{}

TransformationProcess Transformation::update(uint32 uiDiff)
{
    if (m_uiTimer < uiDiff)
    {
        uiDiff = m_uiTimer;
        m_uiTimer = 0;
    }
    else
        m_uiTimer -= uiDiff;
    _update(uiDiff);
    if (!m_uiTimer)
        return DONE;
    return IN_PROGRESS;
}

/*#####
# Move
#####*/
Move::Move(uint32 uiTime, Object *pOwner, Int32Point range) : m_Range(range), Transformation(uiTime, pOwner)
{
    m_RangePerMSEC.x = static_cast<double>(m_Range.x) / getTimeRemain();
    m_RangePerMSEC.y = static_cast<double>(m_Range.y) / getTimeRemain();
}

void Move::_update(uint32 uiDiff)
{
    Int32Point temp;
    if (getTimeRemain())
    {
        m_RangeBuffer.x += m_RangePerMSEC.x*uiDiff;
        m_RangeBuffer.y += m_RangePerMSEC.y*uiDiff;
        temp.x = static_cast<int32>(m_RangeBuffer.x);
        temp.y = static_cast<int32>(m_RangeBuffer.y);
        m_RangeBuffer.x -= temp.x;
        m_RangeBuffer.y -= temp.y;
        m_Range -= temp;
    }
    else
        temp = m_Range;
    getOwner()->setPosition(getOwner()->getPosition()+temp);
}

/*#####
# TransformationHolder
#####*/
TransformationHolder::TransformationHolder()
{
    for (TransformationArray::iterator itr = m_Transformations.begin(); itr != m_Transformations.end(); ++itr)
        *itr = TransformationPtrVectorPtr(new TransformationPtrVector());
}

void TransformationHolder::updateTransformations(uint32 uiDiff)
{
    // update transformations
    for (TransformationArray::iterator itr = m_Transformations.begin(); itr != m_Transformations.end(); ++itr)
    {
        if (!(*itr)->empty() && (*itr)->front()->update(uiDiff) == DONE)
            (*itr)->erase((*itr)->begin());
    }
}

void TransformationHolder::addTransformation(MovePtr &pTransformation)
{
    if (pTransformation)
        m_Transformations.at(TRANSFORMATION_MOVE)->push_back(TransformationPtr(pTransformation.release()));
}