#include "WorldObjectInfoData.h"

namespace MAP
{
    namespace MAP_DATA
    {
        uint32 WorldObjectInfoData::count() const
        {
            return static_cast<uint32>(m_WorldObjectInfos.size());
        }

        bool WorldObjectInfoData::isEmpty() const
        {
            return m_WorldObjectInfos.empty();
        }

        void WorldObjectInfoData::clear()
        {
            m_WorldObjectInfos.clear();
        }

        WorldObjectInfo* WorldObjectInfoData::addWorldObject(uint32 ID, const GEOMETRY::Point<int32>& pos, MapObjectLayer layer, MapDirection direction)
        {
            WorldObjectInfo info(_getNewGUID());
            info.setWorldObjectID(ID);
            info.setPosition(pos);
            info.setLayer(layer);
            info.setDirection(direction);
            
            m_WorldObjectInfos.push_back(info);
            return &m_WorldObjectInfos.back();
        }

        void WorldObjectInfoData::setWorldObject(GUID guid, const WorldObjectInfo& info)
        {
            if (auto pInfo = getWorldObject(guid))
                *pInfo = info;
        }

        const WorldObjectInfo* WorldObjectInfoData::getWorldObject(GUID guid) const
        {
            for (auto& info : m_WorldObjectInfos)
            {
                if (guid == info.getGUID())
                    return &info;
            }
            return nullptr;
        }

        WorldObjectInfo* WorldObjectInfoData::getWorldObject(GUID guid)
        {
            return const_cast<WorldObjectInfo*>(const_cast<const WorldObjectInfoData&>(*this).getWorldObject(guid));
        }

        void WorldObjectInfoData::removeWorldObject(GUID guid)
        {
            for (auto itr = m_WorldObjectInfos.begin(); itr != m_WorldObjectInfos.end(); ++itr)
            {
                if (guid == itr->getGUID())
                {
                    m_WorldObjectInfos.erase(itr);
                    break;
                }
            }
        }

        uint32 WorldObjectInfoData::_getNewGUID() const
        {
            if (isEmpty())
                return 1;
            return m_WorldObjectInfos.back().getGUID() + 1;
        }
    }
}
