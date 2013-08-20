#include "AnimationHolder.h"

using namespace MAP;
using namespace OBJECT;
using namespace ANIMATION;

void AnimationHolder::update(uint32 uiDiff)
{
    if (!m_pAnimation)
        return;

    if (m_uiTimer < uiDiff)
    {
        ++m_uiCurrentFrame;
        if (m_uiCurrentFrame == m_pAnimation->getFrameCount())
            m_uiCurrentFrame = 0;
        _changeCurrentFrame(m_uiCurrentFrame);
    }
    else
        m_uiTimer -= uiDiff;
}

void AnimationHolder::changeAnimation(DATABASE::ConstAnimationPrototypePtr pAnimation)
{
    m_uiCurrentFrame = 0;
    m_pAnimation = pAnimation;
    _changeCurrentFrame(m_uiCurrentFrame);
}

void AnimationHolder::_changeCurrentFrame(uint32 uiFrame)
{
    if (!m_pAnimation)
        return;
    DATABASE::AnimationPrototype::Frame frame;
    if (m_pAnimation->getFrame(uiFrame, frame))
        m_uiTimer = static_cast<uint32>(static_cast<double>(frame.m_uiMsecTime*100)/m_uiAnimationSpeed);
    else
        m_uiTimer = 0;
}