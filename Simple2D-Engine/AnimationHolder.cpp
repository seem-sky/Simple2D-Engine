#include "AnimationHolder.h"

using namespace MAP;
using namespace OBJECT;
using namespace ANIMATION;
using namespace DATABASE::ANIMATION;

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

void AnimationHolder::changeAnimation(const AnimationPrototype *pAnimation)
{
    m_uiCurrentFrame = 0;
    m_pAnimation = pAnimation;
    _changeCurrentFrame(m_uiCurrentFrame);
}

void AnimationHolder::_changeCurrentFrame(uint32 uiFrame)
{
    if (!m_pAnimation)
        return;
    Frame frame;
    if (m_pAnimation->getFrame(uiFrame, frame))
        m_uiTimer = static_cast<uint32>(static_cast<double>(frame.getTimeInMsec()*100)/m_uiAnimationSpeed);
    else
        m_uiTimer = 0;
}