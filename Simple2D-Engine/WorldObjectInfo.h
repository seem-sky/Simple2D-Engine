#ifndef WORLD_OBJECT_INFO_H
#define WORLD_OBJECT_INFO_H

#include <Typedefs.h>

namespace MAP
{
    namespace MAP_DATA
    {
        enum class MapDirection
        {
            UP,
            LEFT,
            DOWN,
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
            void setPosition(Int32Point position);
            Int32Point getPosition() const;
            GUID getGUID() const;

        private:
            uint32 m_ObjectID = 0;
            GUID m_GUID = 0;
            Int32Point m_Position;
            MapDirection m_Direction = MapDirection::DOWN;
            MapObjectLayer m_Layer = MapObjectLayer::MIDDLE;
        };
    }
}
#endif
