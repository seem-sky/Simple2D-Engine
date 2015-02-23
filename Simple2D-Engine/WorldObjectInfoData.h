#ifndef WORLD_OBJECT_DATA_H
#define WORLD_OBJECT_DATA_H

#include "WorldObjectInfo.h"
#include <memory>

namespace MAP
{
    namespace MAP_DATA
    {
        class WorldObjectException : public std::runtime_error
        {
        public:
            WorldObjectException(const std::string& msg);
            WorldObjectException(const char* msg);
        };

        class WorldObjectInfoData
        {
        private:
            GUID _getNewGUID() const;

            WorldObjectInfoData(const WorldObjectInfoData& other) = delete;
            WorldObjectInfoData& operator=(const WorldObjectInfoData& other) = delete;

        public:
            WorldObjectInfoData() = default;

            uint32 count() const;
            bool isEmpty() const;
            void clear();

            WorldObjectInfo* addWorldObject(const WorldObjectInfo& info);
            WorldObjectInfo* addWorldObject(uint32 ID, const GEOMETRY::Point<int32>& pos, MapObjectLayer layer, MapDirection direction = MapDirection::down);
            WorldObjectInfo* getWorldObject(GUID guid);
            const std::vector<std::unique_ptr<WorldObjectInfo>>& getWorldObjects() const;
            const WorldObjectInfo* getWorldObject(GUID guid) const;
            bool hasWorldObject(GUID guid) const;
            void removeWorldObject(GUID guid);

        private:
            std::vector<std::unique_ptr<WorldObjectInfo>> m_WorldObjectInfos;
        };
    }
}
#endif
