#ifndef ANIMATION_MODULE_H
#define ANIMATION_MODULE_H

#include <Global.h>
#include <vector>

namespace DATABASE
{
	namespace PROTOTYPE
	{
		namespace MODULE
		{
			namespace ANIMATION
			{
				enum class VisualType
				{
					SPRITE,
					ANIMATION
				};

				class AnimationInfo
				{
				public:
					AnimationInfo(uint32 ID = 0, VisualType visualType = VisualType::SPRITE, uint32 animationTypeID = 0)
						: m_ID(ID), m_VisualType(visualType), m_AnimationTypeID(animationTypeID)
					{}

					bool isValid() const { return m_ID && m_AnimationTypeID; }

					uint32 m_ID;
					VisualType m_VisualType;
					uint32 m_AnimationTypeID;
				};

				class AnimationModule
				{
				public:
					inline const AnimationInfo& getAnimationInfo(uint32 uiIndex) const { return m_AnimationInfos.at(uiIndex); }
					inline AnimationInfo& getAnimationInfo(uint32 uiIndex) { return m_AnimationInfos.at(uiIndex); }
					void setAnimationInfo(uint32 uiIndex, AnimationInfo& animationInfo);

					inline uint32 getAnimationCount() const { return static_cast<uint32>(m_AnimationInfos.size()); }
					void setAnimationCount(uint32 uiCount) { m_AnimationInfos.resize(uiCount); }

				private:
					typedef std::vector<AnimationInfo> AnimationInfoVector;
					AnimationInfoVector m_AnimationInfos;
				};
			}
		}
	}
}
#endif

