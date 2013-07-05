#include "TransformationHolder.h"
#include <functional>

using namespace TRANSFORMATION;

/*#####
# Transformation
#####*/
Transformation::Transformation(uint32 uiTime) : m_uiTimer(uiTime)
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
Move::Move(uint32 uiTime, Int32Point &position, Int32Point range) : m_Position(position), m_Range(range), Transformation(uiTime)
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
    m_Position += temp;
}

/*#####
# TransformationHolder
#####*/
void TransformationHolder::updateTransformations(uint32 uiDiff)
{
    typedef std::vector<TransformationPtrList::iterator> TransformationPtrListIteratorVector;
    TransformationPtrListIteratorVector itrs;
    for (auto itr = m_Transformations.begin(); itr != m_Transformations.end(); ++itr)
    {
        if ((*itr)->update(uiDiff) == TRANSFORMATION::DONE)
            itrs.push_back(itr);
    }
    // remove finished transformations
    for (auto itr : itrs)
        m_Transformations.erase(itr);
}

void TransformationHolder::addTransformation(TransformationPtr &pTransformation)
{
    m_Transformations.push_back(std::move(pTransformation));
}