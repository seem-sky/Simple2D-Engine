#ifndef WORLD_OBJECT_INFO_H
#define WORLD_OBJECT_INFO_H

#include <Typedefs.h>

namespace MAP
{
    namespace MAP_DATA
    {
        enum class MapDirection
        {
            DOWN,
            LEFT,
            UP,
            RIGHT
        };

        enum class MapObjectLayer
        {
            BELOW_BACKGROUND,
            LOWER,
            MIDDLE,
            UPPER,
            ABOVE_FOREGROUND
        };
        const uint32 MAX_OBJECT_LAYER = 5;

        typedef uint32 GUID;

        // map objects
        class WorldObjectInfo
        {
        public:
            WorldObjectInfo(GUID guid);

            bool isValid() const;

            void setWorldObjectID(uint32 ID);
            uint32 getWorldObjectID() const;
            void setLayer(MapObjectLayer layer);
            MapObjectLayer getLayer() const;
            void setDirection(MapDirection direction);
            MapDirection getDirection() const;
            void setPosition(GEOMETRY::Point<int32> position);
            GEOMETRY::Point<int32> getPosition() const;
            GUID getGUID() const;

        private:
            uint32 m_ObjectID = 0;
            GUID m_GUID = 0;
            GEOMETRY::Point<int32> m_Position;
            MapDirection m_Direction = MapDirection::DOWN;
            MapObjectLayer m_Layer = MapObjectLayer::MIDDLE;
        };
    }
}
#endif
