#include "WorldObjectInfoData.h"

namespace MAP
{
    namespace MAP_DATA
    {
        WorldObjectException::WorldObjectException(const std::string& msg) : std::runtime_error(msg)
        {}

        WorldObjectException::WorldObjectException(const char* msg) : std::runtime_error(msg)
        {}

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

        WorldObjectInfo* WorldObjectInfoData::addWorldObject(const WorldObjectInfo& info)
        {
            if (info.getGUID() == 0)
                throw WorldObjectException("GUID can not be 0!");
            if (auto pObj = getWorldObject(info.getGUID()))
                throw WorldObjectException("GUID already exists.");
            m_WorldObjectInfos.push_back(std::unique_ptr<WorldObjectInfo>(new WorldObjectInfo(info)));
            return m_WorldObjectInfos.back().get();
        }

        WorldObjectInfo* WorldObjectInfoData::addWorldObject(uint32 ID, const GEOMETRY::Point<int32>& pos, MapObjectLayer layer, MapDirection direction)
        {
            std::unique_ptr<WorldObjectInfo> info(new WorldObjectInfo(_getNewGUID()));
            info->setID(ID);
            info->setPosition(pos);
            info->setLayer(layer);
            info->setDirection(direction);

            m_WorldObjectInfos.push_back(std::move(info));
            return m_WorldObjectInfos.back().get();
        }

        const WorldObjectInfo* WorldObjectInfoData::getWorldObject(GUID guid) const
        {
            for (auto& info : m_WorldObjectInfos)
            {
                if (guid == info->getGUID())
                    return info.get();
            }
            return nullptr;
        }

        WorldObjectInfo* WorldObjectInfoData::getWorldObject(GUID guid)
        {
            return const_cast<WorldObjectInfo*>(const_cast<const WorldObjectInfoData&>(*this).getWorldObject(guid));
        }

        const std::vector<std::unique_ptr<WorldObjectInfo>>& WorldObjectInfoData::getWorldObjects() const
        {
            return m_WorldObjectInfos;
        }

        void WorldObjectInfoData::removeWorldObject(GUID guid)
        {
            for (auto itr = m_WorldObjectInfos.begin(); itr != m_WorldObjectInfos.end(); ++itr)
            {
                if (guid == (*itr)->getGUID())
                {
                    m_WorldObjectInfos.erase(itr);
                    break;
                }
            }
        }

        GUID WorldObjectInfoData::_getNewGUID() const
        {
            if (isEmpty())
                return 1;
            return m_WorldObjectInfos.back()->getGUID() + 1;
        }
    }
}
