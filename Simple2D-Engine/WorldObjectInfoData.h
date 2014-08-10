#ifndef WORLD_OBJECT_DATA_H
#define WORLD_OBJECT_DATA_H

#include "WorldObjectInfo.h"

namespace MAP
{
    namespace MAP_DATA
    {
        class WorldObjectInfoData
        {
        private:
            GUID _getNewGUID() const;

        public:
            uint32 count() const;
            bool isEmpty() const;
            void clear();

            WorldObjectInfo* addWorldObject(uint32 ID, const GEOMETRY::Point<int32>& pos, MapObjectLayer layer, MapDirection direction = MapDirection::DOWN);
            WorldObjectInfo* getWorldObject(GUID guid);
            const WorldObjectInfo* getWorldObject(GUID guid) const;
            void setWorldObject(GUID guid, const WorldObjectInfo& info);
            void removeWorldObject(GUID guid);

        private:
            std::list<WorldObjectInfo> m_WorldObjectInfos;
        };
    }
}
#endif
