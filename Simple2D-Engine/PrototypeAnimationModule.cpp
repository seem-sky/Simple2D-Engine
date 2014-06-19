#include "PrototypeAnimationModule.h"

namespace DATABASE
{
	namespace PROTOTYPE
	{
		namespace MODULE
		{
			namespace ANIMATION
			{
				void AnimationModule::setAnimationInfo(uint32 index, AnimationInfo& animationInfo)
				{
					if (index >= m_AnimationInfos.size())
						m_AnimationInfos.resize(index + 1);
					//// do not change animation type id if its an standard entry
					//if (index < getMinimumAnimationCount())
					//{
					//	auto &info = m_AnimationInfos.at(index);
					//	info.m_ID = animationInfo.m_ID;
					//	info.m_VisualType = animationInfo.m_VisualType;
					//}
					//else
					m_AnimationInfos.at(index) = animationInfo;
				}
			}
		}
	}
}