#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "Object.h"
#include "DatabaseMgr.h"
#include <unordered_map>
#include "AnimationHolder.h"

namespace MAP
{
    namespace OBJECT
    {
        typedef std::unordered_map<uint32, uint32> UInt32UInt32UMap;
        typedef DATABASE::MAP_STRUCTURE::MapDirection MapDirection;
        class WorldObject : public Object
        {
        private:
            void _setupFromPrototype(DATABASE::ConstWorldObjectPrototypePtr pWorldObject);
            void _setCurrentAnimation(uint32 pose);

        public:
            WorldObject(uint32 GUID = 0, DATABASE::ConstWorldObjectPrototypePtr pWorldObject = DATABASE::ConstWorldObjectPrototypePtr());

            inline DATABASE::ConstAnimationPrototypePtr getCurrentAnimation() const { return m_AnimationHolder.getCurrentAnimation(); }
            inline uint32 getCurrentFrame() const { return m_AnimationHolder.getCurrentFrame(); }

            inline void setDirection(MapDirection direction) { m_Direction = direction; }
            inline MapDirection getDirection() const { return m_Direction; }

            inline void setDBMgr(DATABASE::ConstDatabaseMgrPtr pDBMgr) { m_pDBMgr = pDBMgr; _setCurrentAnimation(getDirection()); }

            virtual void update(uint32 uiDiff);

        private:
            ANIMATION::AnimationHolder m_AnimationHolder;
            UInt32UInt32UMap m_Animations;
            DATABASE::MAP_STRUCTURE::MapDirection m_Direction;
            UInt32Rect m_BoundingRect;
            DATABASE::ConstDatabaseMgrPtr m_pDBMgr;
        };
        typedef std::shared_ptr<WorldObject> WorldObjectPtr;
    }
}
#endif