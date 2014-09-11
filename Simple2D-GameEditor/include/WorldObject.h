#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "Object.h"
#include "DatabaseMgr.h"
#include <unordered_map>
#include "AnimationHolder.h"
#include "WorldObjectInfo.h"

namespace MAP
{
    namespace OBJECT
    {
        typedef std::unordered_map<uint32, uint32> UInt32UInt32UMap;
        class WorldObject : public Object
        {
        private:
            void _setupFromPrototype(const DATABASE::PROTOTYPE::WORLD_OBJECT::WorldObjectPrototype* pWorldObject);
            void _setCurrentAnimation(uint32 pose);

        public:
            WorldObject(const DATABASE::DatabaseMgr& pDBMgr, uint32 GUID = 0, uint32 ID = 0);

            inline const DATABASE::PROTOTYPE::ANIMATION::AnimationPrototype* getCurrentAnimation() const { return m_AnimationHolder.getCurrentAnimation(); }
            inline uint32 getCurrentFrame() const { return m_AnimationHolder.getCurrentFrame(); }

            inline void setDirection(MAP_DATA::MapDirection direction) { m_Direction = direction; }
            inline MAP_DATA::MapDirection getDirection() const { return m_Direction; }

            virtual void update(uint32 uiDiff);

        private:
            ANIMATION::AnimationHolder m_AnimationHolder;
            UInt32UInt32UMap m_Animations;
            MAP_DATA::MapDirection m_Direction;
            GEOMETRY::Rectangle<int32> m_BoundingRect;
            const DATABASE::DatabaseMgr& m_DBMgr;
        };
    }
}
#endif