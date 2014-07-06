#include "WorldObjectInfo.h"

namespace MAP
{
    namespace MAP_DATA
    {
        WorldObjectInfo::WorldObjectInfo(GUID guid) : m_GUID(guid)
        {}

        bool WorldObjectInfo::isValid() const
        {
            return !m_GUID || !m_ObjectID;
        }

        void WorldObjectInfo::setWorldObjectID(uint32 ID)
        {
            m_ObjectID = ID;
        }

        uint32 WorldObjectInfo::getWorldObjectID() const
        {
            return m_ObjectID;
        }

        void WorldObjectInfo::setLayer(MapObjectLayer layer)
        {
            m_Layer = layer;
        }

        MapObjectLayer WorldObjectInfo::getLayer() const
        {
            return m_Layer;
        }

        void WorldObjectInfo::setDirection(MapDirection direction)
        {
            m_Direction = direction;
        }

        MapDirection WorldObjectInfo::getDirection() const
        {
            return m_Direction;
        }

        void WorldObjectInfo::setPosition(Int32Point position)
        {
            m_Position = position;
        }

        Int32Point WorldObjectInfo::getPosition() const
        {
            return m_Position;
        }

        GUID WorldObjectInfo::getGUID() const
        {
            return m_GUID;
        }
    }
}