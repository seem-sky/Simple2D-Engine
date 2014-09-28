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

        void WorldObjectInfo::setID(uint32 ID)
        {
            m_ObjectID = ID;
        }

        uint32 WorldObjectInfo::getID() const
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

        void WorldObjectInfo::setPosition(GEOMETRY::Point<int32> position)
        {
            m_Position = position;
        }

        GEOMETRY::Point<int32> WorldObjectInfo::getPosition() const
        {
            return m_Position;
        }

        GUID WorldObjectInfo::getGUID() const
        {
            return m_GUID;
        }
    }
}